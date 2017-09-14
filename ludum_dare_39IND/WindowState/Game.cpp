#include "Game.h"
#include "../Event/EventManager.h"
#include <SFML/Window/Keyboard.hpp>
#include "../Event/PrintEvent.h"
#include "../WindowState/GeneralData.h"
#include "../System/AllSystemsHeader.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "../Helper/Constant.h"
#include "../Helper/Math.h"
#include "../StateEvent/StateEventManager.h"
#include "../Component/PlayerComponent.h"
#include "../Component/Body2dComponent.h"

#include "../Audio/AudioManager.h"

#include <Thor\Particles\Emitters.hpp>
#include <Thor\Math\Distributions.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/OpenGL.hpp>

Game::Game(GeneralData* generalData, sf::RenderWindow& window, 
	lua_State* luaState, StateEventManager* stateEventManager)
:mWindow(window),
mGeneralData(generalData),
mLuaState(luaState),
mStateEventManager(stateEventManager),
mSfDebugB2Draw(mWindow),
mEngineContactListener(*mGeneralData->mEntityManager.get()),
mDoEventCheck(true)
{
	//generalData->mTiledMap->getMapSize();

	sf::Vector2f real_map_size = generalData->mTiledMap->getMapSize() * generalData->mTiledMap->getTileSize().x;

	//m_screen_texture.create(real_map_size.x, real_map_size.y);
	m_screen_texture.create(window.getSize().x, window.getSize().y);

	m_final_screen_texture.create(window.getSize().x, window.getSize().y);

	/*EventManager::getInstance()->addListener<PrintEvent>(Utility::convertPointerToAddress(this), 
			std::bind(&Game::bindPrintEvent, this, std::placeholders::_1));*/
	//mWaveEntitySpawner.readWaveScript(Constant::scriptDir + "Wave1Script.lua", luaState);
	mGeneralData->mB2World->SetContactListener(&mEngineContactListener);
	mBGSprite.setTexture(*mGeneralData->mTexturesStringmanager->
		loadTextures(Constant::textureDir + "wallpaper_3840.png", true));
	//mBGSprite.setTextureRect(sf::FloatRect(0.f, 0.f, 32.f - 0.0075, 32.f - 0.0075));
	

	/*sf::Vector2f realMapSize = mGeneralData->mTiledMap->getMapSize() * mGeneralData->mTiledMap->getTileSize().x;
	sf::Vector2i realMapSizeInt(realMapSize);

	mBGSprite.setTextureRect(sf::IntRect(0, 0, realMapSizeInt.x, realMapSizeInt.y));*/



	
	//mGeneralData->mB2World->SetDebugDraw(&mSfDebugB2Draw);
}


Game::~Game()
{
}



void Game::draw()
{
	SystemManager* systemManager = mGeneralData->getSystemManager();

	m_screen_texture.clear();
	m_screen_texture.setView(mWindow.getView());
	
	//mWindow.draw(mBGSprite);
	m_screen_texture.draw(mBGSprite);

	mStateEventManager->drawStateEvent(m_screen_texture, sf::RenderStates(), States::Game);
	//mGeneralData->mFourTree->draw(mWindow);

	


	systemManager->draw<RenderingSpriteSystem>(m_screen_texture, sf::RenderStates());
	systemManager->draw<ThorParticleSystem>(m_screen_texture, sf::RenderStates());

	m_screen_texture.display();

	//mWindow.draw(sf::Sprite(m_screen_texture.getTexture()));

	m_final_screen_texture.clear();

	m_bloom_effect.apply(m_screen_texture, m_final_screen_texture);
	m_final_screen_texture.display();

	sf::Sprite tex_sprite(m_final_screen_texture.getTexture());

	tex_sprite.setOrigin(tex_sprite.getTextureRect().width / 2.f, tex_sprite.getTextureRect().height / 2.f);

	tex_sprite.setPosition(mWindow.getView().getCenter());

	mWindow.draw(tex_sprite);

	//mWindow.draw(m_particle_system);

	//systemManager->draw<AutomaticMovementSystem>(mWindow, sf::RenderStates());
	//mGeneralData->mB2World->DrawDebugData();
		
}

void Game::init_opengl()
{
	
}

void Game::update(sf::Time dt)
{
	
	SystemManager* systemManager = mGeneralData->getSystemManager();

	systemManager->update<Box2DWorldSystem>(mGeneralData->mEntityManager.get(), sf::Time::Zero);

	EntityManager* entityManager = mGeneralData->getEntityManager();
	Camera* camera = mGeneralData->mCamera.get();
	b2World* box2dWorld = mGeneralData->mB2World.get();
	//GridMap* gridMap = mGeneralData->mGridMap.get();
	
	systemManager->setSystemsBoundRect(camera->getBoundingRect());
	

	const sf::View& cameraView = camera->getView();
	mWindow.setView(cameraView);

	
	entityManager->cleanEntityToBeRemoved(box2dWorld);
	systemManager->update<RealTimeInputSystem>(entityManager, dt);
	systemManager->update<PlayerSystem>(entityManager, dt);
	mStateEventManager->updateOnState(mWindow, dt, States::Game);
	camera->update(dt);
		
	entityManager->processQueuedEntityData(mGeneralData->getComponentArranger(), mLuaState);
	
	systemManager->update<ObstacleSystem>(entityManager, dt);
	systemManager->update<PlanetSpawnSystem>(entityManager, dt);
	systemManager->update<ScriptUpdateSystem>(entityManager, dt);
	systemManager->update<ScriptAISystem>(entityManager, dt);
	systemManager->update<AutomaticMovementSystem>(entityManager, dt);
	systemManager->update<JumpSystem>(entityManager, dt);
	//systemManager->update<AutomaticMovementSystem>(entityManager, dt);
	systemManager->update<AutoBody2dMovementSystem>(entityManager, dt);
	systemManager->update<AnimationSystem>(entityManager, dt);
	systemManager->update<Box2DWorldSystem>(entityManager, dt);
	systemManager->update<RenderingSpriteSystem>(entityManager, dt);
	systemManager->update<ThorParticleSystem>(entityManager, dt);
		
	
//	mGeneralData->mPlayerEntity->getComp<PlayerComponent>()->setIsInteractingThisFrame(false);
}


void Game::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::GainedFocus) mDoEventCheck = true;
	else if (event.type == sf::Event::LostFocus) mDoEventCheck = false;

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
		Utility::printVector(
			mGeneralData->mPlayerEntity->getComp<Body2dComponent>()->getSfLinearVelocity(), true);

	if (!mDoEventCheck) 
		return;

	mGeneralData->mSystemManager->handleEvent<RealTimeInputSystem>(mWindow, event);
	mGeneralData->mSystemManager->handleEvent<PlayerSystem>(mWindow, event);
	mStateEventManager->handleStateEvent(mWindow, event, States::Game);
}

void Game::processCameraInput(sf::Time dt)
{
	sf::Vector2f finalCameraV;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		finalCameraV.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		finalCameraV.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		finalCameraV.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		finalCameraV.x = 1.f;
	Camera* camera = mGeneralData->mCamera.get();

	camera->moveCamera(finalCameraV, 300, dt);
}

void Game::bindPrintEvent(const PrintEvent* printEvent)
{
	std::cout << printEvent->mRandomInteger << std::endl;
}
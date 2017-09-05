#include "Application.h"
#include "WindowState/MenuState.h"
#include "Helper/Constant.h"
#include "WindowState/LoadingTask.h"
#include <LuaBridge.h>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include "Entity/Entity.h"
#include "Helper/Utility.h"
#include "Helper/Math.h"
#include "Component/AllEntityComponentsHeader.h"
#include "System/AllSystemsHeader.h"
#include "Event/EventManager.h"
#include "Event/AllEventsHeader.h"
#include "Map/GridMap.h"
#include "Map/TiledMapSystem.h"
#include "Tile/TileSidesSensorGenerator.h"
#include "Audio/AudioManager.h"

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application(luabridge::LuaRef windowSettingTable, lua_State* luaState)
:mWindow(sf::VideoMode(windowSettingTable["width"].cast<std::size_t>(), 
	windowSettingTable["height"].cast<std::size_t>()), "", sf::Style::Resize, sf::ContextSettings(0, 0, 0, 1, 1, 0)),
mFont(),
mStacks(State::Context(mWindow, mFont, luaState))
{
	mFont.loadFromFile("Media/Fonts/Sansation.ttf");
	
	//mWindow.setSize(sf::Vector2u(1366, 768));
	mWindow.setTitle(windowSettingTable["title"].cast<std::string>());
	mWindow.setFramerateLimit(windowSettingTable["fps"].cast<std::size_t>());
	mWindow.setKeyRepeatEnabled(false);
	MenuState::Ptr menuState(new MenuState(mStacks, mStacks.getCurrentStateContext()));
	mStacks.pushState(std::move(menuState));

	bindEngineInterfacesToLuaScript();
		

	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	AudioManager::getInstance()->initializeAudioMap(luaState);
}


Application::~Application()
{
}


void Application::run()
{
	sf::Time limitTime = sf::Time::Zero;
	sf::Clock clock;
	while (mWindow.isOpen()){
		sf::Time dt = clock.restart();
		limitTime += dt;
		while (TimePerFrame <= limitTime){
			limitTime -= TimePerFrame;
			EventManager::getInstance()->processQueuedEvents();
			processInputs();
			update(TimePerFrame);
			if (mStacks.isEmpty())
				mWindow.close();
		}
		updateStatistics(dt);
		render();
	}
}

void Application::update(sf::Time dt)
{
	mStacks.update(dt);
}

void Application::processInputs()
{
	sf::Event event;
	while (mWindow.pollEvent(event)){
		mStacks.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::render()
{
	mWindow.clear();

	mStacks.draw();
	//mWindow.setView(mWindow.getDefaultView());
	sf::View view = mWindow.getView();
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, 0.f, 0.f);

	mStatisticsText.setPosition(viewPort.left + 5.f, viewPort.top + 5.f);
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" //+
			/*"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us"*/);
		//mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsUpdateTime = sf::Time::Zero;
		mStatisticsNumFrames = 0;
	}
}

void Application::bindEngineInterfacesToLuaScript()
{
	lua_State* luaState = mStacks.getLuaState();

	/*Registering LUABRIDGE classes*/

	/**/

	/*Registering SFML Api*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Time>("sfTime")
		.addConstructor<void(*)(void)>()

		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Vector2f>("sfVector2f")
		//.addConstructor<void(*)(void)>()
		.addConstructor<void(*)(float, float)>()
		//.addConstructor<void(*)(float, float)>()
		.addData("x", &sf::Vector2f::x)
		.addData("y", &sf::Vector2f::y)
		//.addProperty("", sf::Vector3)
		.endClass();
	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Vector2i>("sfVector2i")
		.addConstructor<void(*)(void)>()
		.addData("x", &sf::Vector2i::x)
		.addData("y", &sf::Vector2i::y)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Transformable>("sfTransformable")
		.addConstructor<void(*)(void)>()
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::FloatRect>("sfFloatRect")
		.addConstructor<void(*)(void)>()
		.addData("width", &sf::FloatRect::width)
		.addData("height", &sf::FloatRect::height)
		.addData("top", &sf::FloatRect::top)
		.addData("left", &sf::FloatRect::left)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::IntRect>("sfIntRect")
		.addConstructor<void(*)(void)>()
		.addData("left", &sf::IntRect::left)
		.addData("top", &sf::IntRect::top)
		.addData("width", &sf::IntRect::width)
		.addData("height", &sf::IntRect::height)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<sf::Texture>("sfTexture")
		.endClass();
	/**/

	/*Registering C++ standard library*/
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Std")
		.beginClass<std::string>("StdString")
		.endClass()
		.endNamespace();

	/**/

	/*Registering Utility function*/

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("EngineUtil")
		.addFunction("printStringMessage", &Utility::printStringToCmd)
		.addFunction("offSetRect1AndCheckRect2", &Utility::offSetRect1AndCheckRect2Script)
		.addFunction("randomRange", &Utility::randomRange)
		.addFunction("randomInt", &Utility::randomInt)
		.endNamespace();


	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("EngineMath")
		.addFunction("vectorLength", &Math::vectorLength)
		.addFunction("unitVector", &Math::unitVectorScript)
		.addFunction("minusVector", &Math::minusVector)
		.addFunction("plusVector", &Math::plusVector)
		.addFunction("getDotProduct", &Math::getDotProduct)
		.addFunction("vectorToDegree", &Math::vectorToDegree)
		.addFunction("degreeToVector", &Math::degreeToVector)
		.addFunction("multiplyVector", &Math::multiplyVector)
		.addFunction("multiplyVectorByFloat", &Math::multiplyVectorByFloat)
		.addFunction("minusVectorI", &Math::minusVectorI)
		.addFunction("plusVectorI", &Math::plusVectorI)
		.addFunction("getDotProductDegree", &Math::getDotProductDegree)
		.addFunction("minusDegree", &Math::minusDegree)
		.addFunction("plusDegree", &Math::plusDegree)
		.addFunction("transformPointWithAngle", &Math::transformPointWithAngle)
		.endNamespace();

	/**/

	/*Registering functionCaller class*/


	/**/

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("EngineConstant")
		.addVariable("ScriptDir", &Constant::scriptScriptDir, false)
		.endNamespace();
		
	/*Register coded C++ event*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<EventManager>("EventManager")
		.addStaticFunction("getInstance", &EventManager::getInstance)

		.addFunction("addLuaListener", &EventManager::addLuaListener)
		.addFunction("queueEvent", &EventManager::queueScriptEvent)
		.addFunction("deleteLuaListener", &EventManager::deleteLuaListener)
		.endClass();
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Event")
		.beginClass<BaseEvent>("BaseEvent")
		//.addConstructor<void(*)(void)>()
		.endClass()
		.deriveClass<Event<PrintEvent>, BaseEvent>("Event<PrintEvent>")
		.endClass()
		.deriveClass<PrintEvent, Event<PrintEvent>>("PrintEvent")
		//.beginClass<PrintEvent>("PrintEvent")
		.addConstructor<void(*)(int)>()
		.addData("mRandomInteger", &PrintEvent::mRandomInteger)
		.endClass()
		.deriveClass<Event<PlayerCollidesSolidEntityEvent>, BaseEvent>("Event<PlayerCollidesSolidEntityEvent>")
		.endClass()
		.deriveClass<PlayerCollidesSolidEntityEvent, 
			Event<PlayerCollidesSolidEntityEvent>>("PlayerCollidesSolidEntityEvent")
			.addConstructor<void(*)()>()
			.addData("mCollidedEntity", &PlayerCollidesSolidEntityEvent::mCollidedEntity)
			.addData("mCollidedFixtureLogic", &PlayerCollidesSolidEntityEvent::mCollidedFixtureLogic)
			.addData("mPlayerFixtureLogic", &PlayerCollidesSolidEntityEvent::mPlayerFixtureLogic)
			
		.endClass()
		.deriveClass<Event<EntityCollidesEntityEvent>, BaseEvent>("Event<EntityCollidesEntityEvent>")
		.endClass()
		.deriveClass<EntityCollidesEntityEvent, Event<EntityCollidesEntityEvent> > 
			("EntityCollidesEntityEvent")
			.addConstructor<void(*)()>()
			.addData("mEntityA", &EntityCollidesEntityEvent::mEntityA)
			.addData("mEntityB", &EntityCollidesEntityEvent::mEntityB)
			.addData("mFixtureLogicA", &EntityCollidesEntityEvent::mFixtureLogicA)
			.addData("mFixtureLogicB", &EntityCollidesEntityEvent::mFixtureLogicB)
		.endClass()
		.endNamespace();
	/*luabridge::getGlobalNamespace(luaState)
	.beginClass<PrintEvent>("PrintEvent")
	.addData("mRandomInteger", &PrintEvent::mRandomInteger)
	.endClass();*/
	/**/

	/*Registering coded C++ classes/function*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ImportedTileData>("ImportedTileData")
		//.addConstructor<void(*)(sf::Texture*, const sf::IntRect&, const sf::Vector2f&, const std::string&, const int)>()
		.addData("mGridMap", &ImportedTileData::mGridMap)
		.addData("mGameObjectType", &ImportedTileData::mGameObjectType)
		.addData("mIntRect", &ImportedTileData::mIntRect)
		.addData("mLayerName", &ImportedTileData::mLayerName)
		.addData("mTileTexture", &ImportedTileData::mTileTexture)
		.addData("mWorldPos", &ImportedTileData::mWorldPos)
		.addData("mTiledMap", &ImportedTileData::mTiledMapSystem)
		.addData("mTileSetId", &ImportedTileData::mTileSetId)
		.addData("mTileId", &ImportedTileData::mTileId)
		.addFunction("isTexturesManagerInt", &ImportedTileData::isTexturesManagerInt)
		.addFunction("isTexturesManagerStr", &ImportedTileData::isTexturesManagerStr)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<EngineObjectInitializer>("EngineObjectInitializer")
		.addData("mTileSidesSensorGen", &EngineObjectInitializer::mTileSidesSensorGen)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TileSidesSensorGenerator>("TileSidesSensorGenerator")
		.addFunction("generateTileSensorSides", 
			&TileSidesSensorGenerator::generateTileSensorSidesScript)
		.addFunction("generatePolygonFixtureData",
			&TileSidesSensorGenerator::generatePolygonFixtureData)
		.endClass();
	/**/

	/*Path Finding classes related begin*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<GridNode>("GridNode")
		.addData("mIsTraverseable", &GridNode::mIsTraverseable)
		.addData("mGridPos", &GridNode::mGridPos)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<GridMap>("GridMap")
		.addFunction("toGrid", &GridMap::toGridScript)
		.addFunction("toCoordinate", &GridMap::toCoordinateScript)
		.addFunction("At", &GridMap::AtScript)
		.addFunction("doesNodeContainsObstacle", &GridMap::doesNodeContainsObstacle)
		.addFunction("getGridSize", &GridMap::getGridSize)
		.addFunction("is_entity_not_obstacled_on_node", &GridMap::is_entity_not_obstacled_on_node)
		.endClass();
	/*Path findng classes related end*/



	/*luabridge::getGlobalNamespace(luaState)
	.beginClass<GridNode>("GridNode")
	.addData("pos", &GridNode::pos)
	.addData("tile", &GridNode::tile)
	.endClass();
	*/

	/*Related to common entity functionality*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<EntityManager>("EntityManager")
		.addFunction("queueEntity", &EntityManager::queueEntityScript)
		.addFunction("queueEntityZ", &EntityManager::queueEntityZScript)
		.addFunction("layerToZIndex", &EntityManager::layerToZIndex)
		.addFunction("pushInteractionFuncData", &EntityManager::pushInteractionFuncDataScript)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Entity>("Entity")
		.addConstructor<void(*)(std::size_t, std::size_t)>()
		.addFunction("getId", &Entity::getId)
		.addFunction("getZIndex", &Entity::getZIndex)
		.addFunction("markEntityToBeRemoved", &Entity::markEntityToBeRemoved)
		.addFunction("isEntityToBeRemoved", &Entity::isEntityToBeRemoved)
		.addFunction("compAnimation", &Entity::nonCreateComp<AnimationComponent>)
		.addFunction("compSprite", &Entity::nonCreateComp<SpriteComponent>)
		
		.addFunction("compCategory", &Entity::nonCreateComp<CategoryComponent>)
		
		.addFunction("compTextDisplay", &Entity::nonCreateComp<TextDisplayComponent>)
		.addFunction("compHealth", &Entity::nonCreateComp<HealthComponent>)
		.addFunction("compScriptUpdate", &Entity::nonCreateComp<ScriptUpdateComponent>)
		.addFunction("compTimer", &Entity::nonCreateComp<TimerComponent>)
		.addFunction("compDestroyable", &Entity::nonCreateComp<DestroyableComponent>)
		
		.addFunction("compAutomaticPath", &Entity::nonCreateComp<AutomaticPathComponent>)
		.addFunction("compPlayer", &Entity::nonCreateComp<PlayerComponent>)
		
		.addFunction("compBody2d", &Entity::nonCreateComp<Body2dComponent>)
		.addFunction("compJump", &Entity::nonCreateComp<JumpComponent>)
		
		.addFunction("compEntityChildren", &Entity::nonCreateComp<EntityChildrenComponent>)
		.addFunction("compInstance", &Entity::nonCreateComp<InstanceComponent>)
		.addFunction("compScriptAI", &Entity::nonCreateComp<ScriptAIComponent>)
		
		.addFunction("compAutoBody2d", &Entity::nonCreateComp<AutoBody2dMovementComponent>)

		.addFunction("compSpaceShip", &Entity::nonCreateComp<SpaceShipComponent>)
		//.addFunction("", &Entity::nonCreateComp<>)
		.endClass();

	/*End common entity functionality*/

	/*Box2D Api registration*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<b2Fixture>("b2Fixture")
		.endClass();

	/**/

	/*Registeing Component classes begin*/
	luabridge::getGlobalNamespace(luaState)
		.beginClass<AnimationComponent>("AnimationComponent")
		.addConstructor<void(*)(Entity*)>()
		.addFunction("setAnimation", &AnimationComponent::setAnimation)
		.addFunction("has_dead_anim_played", &AnimationComponent::has_dead_anim_played)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<SpaceShipComponent>("SpaceShipComponent")
		.addFunction("has_projectile", &SpaceShipComponent::has_projectile)
		.addFunction("shoot_new_projectile", &SpaceShipComponent::shoot_new_projectile)
		.addFunction("shoot_new_projectile_guided", &SpaceShipComponent::shoot_new_projectile_guided)
		.addFunction("can_shoot_missile", &SpaceShipComponent::can_shoot_missile)
		.addFunction("has_missile_ability", &SpaceShipComponent::has_missile_ability)
		.addFunction("reset_missile_allowance", &SpaceShipComponent::reset_missile_allowance)
		.addFunction("allow_missile_to_shoot", &SpaceShipComponent::allow_missile_to_shoot)
		.endClass();


	luabridge::getGlobalNamespace(luaState)
		.beginClass<InstanceComponent>("InstanceComponent")
		.addFunction("callInstanceLuaFunc", &InstanceComponent::callInstanceLuaFunc)
		.endClass();


	luabridge::getGlobalNamespace(luaState)
		.beginClass<SpriteComponent>("SpriteComponent")
		.addFunction("setTexture", &SpriteComponent::setTexture)
		.addFunction("setTextureRect", &SpriteComponent::setTextureRect)
		.addFunction("setTextureRectOrigin", &SpriteComponent::setTextureRectOriginScript)
		.addFunction("setSpriteWorldOrigin", &SpriteComponent::setSpriteWorldOriginScript)
		.addFunction("beganBlink", &SpriteComponent::beganBlink)
		.endClass();

	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TextDisplayComponent>("TextDisplayComponent")
		.addFunction("setString", &TextDisplayComponent::setString)
		.addFunction("incrementString", &TextDisplayComponent::incrementString)
		.endClass();

	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<AutomaticPathComponent>("AutomaticPathComponent")
		.addFunction("setDestToCalculate", &AutomaticPathComponent::setDestToCalculate)
		.addFunction("isAutomaticPathEmpty", &AutomaticPathComponent::isAutomaticPathsEmpty)
		.addFunction("clearAutomaticPath", &AutomaticPathComponent::clearAutomaticPaths)
		.addFunction("isUncalculatedPosValid", &AutomaticPathComponent::isUncalculatedPosValid)
		.endClass();

	/*luabridge::getGlobalNamespace(luaState)
	.beginClass<ScriptAIState>("ScriptAIState")
	.addConstructor<void(*)(Entity*)>()
	.endClass();*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TimerComponent>("TimerComponent")
		.addFunction("updateTimer", &TimerComponent::updateTimer)
		.addFunction("setNewTimeLimitToTimer", &TimerComponent::setNewTimeLimitToTimer)
		.addFunction("isTimerElapsed", &TimerComponent::isTimerElapsed)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<PlayerComponent>("PlayerComponent")
		
		.addFunction("getCurStateEventName", &PlayerComponent::getCurStateEventName)
		.addFunction("set_is_player_in_charge_area", &PlayerComponent::set_is_player_in_charge_area)
		.addFunction("is_player_available", &PlayerComponent::is_player_available)
		.addFunction("increase_drain_power", &PlayerComponent::increase_drain_power)
		
		.endClass();


	luabridge::getGlobalNamespace(luaState)
		.beginClass<CategoryComponent>("CategoryComponent")
		.addFunction("getCategory", &CategoryComponent::getCategory)
		.endClass();




	luabridge::getGlobalNamespace(luaState)
		.beginClass<HealthComponent>("HealthComponent")
		.addFunction("damage", &HealthComponent::damage)
		.addFunction("increaseHealth", &HealthComponent::increaseHealth)
		.addFunction("getCurrentHealth", &HealthComponent::getCurrentHealth)
		.addFunction("setIsImmune", &HealthComponent::setIsImmune)
		.addFunction("hasZeroHealth", &HealthComponent::hasZeroHealth)
		.addFunction("doDirectDamage", &HealthComponent::doDirectDamage)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<DamageData>("DamageData")
		.addData("mDamage", &DamageData::mDamage)
		.addData("mDamagerEntity", &DamageData::mDamagerEntity)
		.addData("mDamagedEntity", &DamageData::mDamagedEntity)
		.addData("mDamageType", &DamageData::mDamageType)
		.addData("mDamagedEntityDefType", &DamageData::mDamagedEntityDefType)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptUpdateComponent>("ScriptUpdateComponent")
		.addFunction("getMemberVariables", &ScriptUpdateComponent::getScriptMemberVariables)
		.addFunction("popScriptStack", &ScriptUpdateComponent::popScriptStack)
		.addFunction("setVarStringValue", &ScriptUpdateComponent::setVarValue<std::string>)
		.addFunction("setVarFloatValue", &ScriptUpdateComponent::setVarValue<float>)
		.addFunction("setVarIntValue", &ScriptUpdateComponent::setVarValue<int>)
		
		.addFunction("getVarStringValue", &ScriptUpdateComponent::getVarValue<std::string>)
		.addFunction("getVarFloatValue", &ScriptUpdateComponent::getVarValue<float>)
		.addFunction("getVarIntValue", &ScriptUpdateComponent::getVarValue<int>)

		.endClass();



	luabridge::getGlobalNamespace(luaState)
		.beginClass<DestroyableComponent>("DestroyableComponent")
		.addFunction("isDestroyed", &DestroyableComponent::isDestroyed)
		.endClass();


	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<FixtureLogic>("FixtureLogic")
		.addData("mFixture", &FixtureLogic::mFixture)
		.addData("mFixtureName", &FixtureLogic::mFixtureName)
		.addFunction("getCategory", &FixtureLogic::getCategory)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Body2dComponent>("Body2dComponent")
		.addFunction("setPosition", &Body2dComponent::setPositionScript)
		.addFunction("setVelocity", &Body2dComponent::setVelocityScript)
		.addFunction("getFacingDir", &Body2dComponent::getFacingDir)
		.addFunction("getLinearVelocity", &Body2dComponent::getSfLinearVelocity)
		.addFunction("getLinearUnitVelocity", &Body2dComponent::getSfLinearUnitVelocity)
		.addFunction("getPosition", &Body2dComponent::getPositionScript)
		.addFunction("getWorldPosition", &Body2dComponent::getSfWorldPosition)
		.addFunction("setMainFixtureVerticesShape", &Body2dComponent::setMainFixtureVerticesShapeScript)
		.addFunction("setFixtureVerticesShape", &Body2dComponent::setFixtureVerticesShapeScript)
		.addFunction("setBaseSpeed", &Body2dComponent::setBaseSpeed)
		.addFunction("getBaseSpeed", &Body2dComponent::getBaseSpeed)
		.addFunction("setAngleDegree", &Body2dComponent::setAngleDegree)
		.addFunction("getAngleDegree", &Body2dComponent::getAngleDegree)
		.addFunction("setFixtureActive", &Body2dComponent::setFixtureActive)
		.endClass();

	


	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<JumpComponent>("JumpComponent")
		.addFunction("getMaxNumOfJumpLimit", &JumpComponent::getMaxNumOfJumpLimit)
		.addFunction("setCurNumberOfJumpLimit", &JumpComponent::setCurNumberOfJumpLimit)
		.addFunction("resetJumpCount", &JumpComponent::resetJumpCount)
		.addFunction("isOnGround", &JumpComponent::isOnGround)
		.addFunction("isJumping", &JumpComponent::isJumping)
		.addFunction("attachGroundEntity", &JumpComponent::attachGroundEntity)
		.addFunction("detachGroundEntity", &JumpComponent::detachGroundEntity)
		.endClass();




	luabridge::getGlobalNamespace(luaState)
		.beginClass<EntityChildrenComponent>("EntityChildrenComponent")
		.addFunction("getParent", &EntityChildrenComponent::getParent)
		.endClass();

	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ScriptAIComponent>("ScriptAIComponent")
		.addFunction("setCurState", &ScriptAIComponent::setCurState)
		.addFunction("getCurAIStateName", &ScriptAIComponent::getCurAIStateName)
		.endClass();
		
	

	luabridge::getGlobalNamespace(luaState)
		.beginClass<AutoBody2dMovementComponent>("AutoBody2dMovementComponent")
		.addFunction("getCurAutoMovBaseDir", &AutoBody2dMovementComponent::getCurAutoMovBaseDir)
		.endClass();

	/*Component classes end here~*/


	/**/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<TiledMapSystem>("TiledMapSystem")
		.addFunction("propertyKeyToValue", &TiledMapSystem::propertyKeyToValue)
		.addFunction("propertyValueWithTileSetId", &TiledMapSystem::propertyValueWithTileSetId)
		.addFunction("objectPropertyValueWithTileSetId", &TiledMapSystem::objectPropertyValueWithTileSetId)
		.addFunction("getObjectData", &TiledMapSystem::getObjectData)
		.endClass();


	luabridge::getGlobalNamespace(luaState)
		.beginClass<TiledMapSystem::ObjectData>("MapObjectData")
		.addData("rotation", &TiledMapSystem::ObjectData::rotation)
		.addFunction("getObjectPropertyValue", &TiledMapSystem::ObjectData::getObjectPropertyValue)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<AudioManager>("AudioManager")
		.addStaticFunction("getInstance", &AudioManager::getInstance)
		.addFunction("playSoundEffect", &AudioManager::playSoundEffect)
		.endClass();


	/*Coded C++ classes end here*/

	luabridge::getGlobalNamespace(luaState)
		.beginClass<luabridge::LuaRef>("LuaRef")
		.endClass();
}

#include "GameState.h"
#include "PauseState.h"
#include "../Audio/AudioManager.h"
#include "GameOverState.h"
#include "../Helper/Utility.h"
#include "../Helper/Constant.h"

GameState::GameState(StateStack& stack, Context context, std::unique_ptr<GeneralData> generalData,
	StateEventManager::ShrPtr& stateEventManagerPtr)
:State(stack, context),
mGeneralData(std::move(generalData)),
mGame(generalData.get(), *context.mWindow, context.mLuaState, stateEventManagerPtr.get()),
mStateEventManagerPtr(stateEventManagerPtr),
mMusicHasntStarted(false),
m_window(*context.mWindow),
m_has_tutorial_skipped(false)
{
	mStateID = States::Game;
	m_tutorial_ui_sprite.setTexture(*mGeneralData->mTexturesStringmanager->
		loadTextures(Constant::texture_ui_dir + "tutorial_UI.png"));
	sf::Vector2u window_size(m_window.getSize());

	if (window_size.x < 1600.f || window_size.y < 900.f)
		m_tutorial_ui_sprite.setScale(0.84f, 0.84f);

	m_tutorial_ui_sprite.setPosition(sf::Vector2f(window_size) / 2.f);
	Utility::centerOrigin(m_tutorial_ui_sprite);
}


GameState::~GameState()
{
	AudioManager::getInstance()->stopMusic();
}


void GameState::draw()
{
	if (!m_has_tutorial_skipped)
		m_window.draw(m_tutorial_ui_sprite);

	if (m_has_tutorial_skipped)
		mGame.draw();
}

bool GameState::update(sf::Time dt)
{
	if (!mMusicHasntStarted){
		AudioManager::getInstance()->playMusic("MainTheme", 50);
		mMusicHasntStarted = true;
	}
	if (!m_has_tutorial_skipped)
		return true;

	mStateEventManagerPtr->checkAnyState(getContext().mLuaState);
	mGame.update(dt);
	if (mGeneralData->mIsGameOver){
		GameOverState::Ptr gameOverState(new GameOverState(*getStateStack(), getContext(), mGeneralData->m_game_over_message));
		requestStackPush(std::move(gameOverState));
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Space)
	{
		m_has_tutorial_skipped = true;
	}

	if (!m_has_tutorial_skipped)
		return true;
	mGame.handleEvent(event);
	if (event.type != sf::Event::KeyPressed)
		return true;
		

	if (event.key.code == sf::Keyboard::Escape){
		//AudioManager::getInstance()->pauseMusic();
		PauseState::Ptr pauseState(new PauseState(*getStateStack(), getContext()));
		requestStackPush(std::move(pauseState));
	}
	return true;
}
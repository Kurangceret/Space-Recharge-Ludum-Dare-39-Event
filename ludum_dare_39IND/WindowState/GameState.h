#pragma once
#include "StateStack.h"
#include "Game.h"
#include "../StateEvent/StateEventManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

class GameState :	public State{
public:
	GameState(StateStack& stack, Context context, std::unique_ptr<GeneralData>,
		StateEventManager::ShrPtr& stateEventManagerPtr);
	virtual ~GameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

private:
	Game mGame;
	sf::RenderWindow& m_window;

	std::unique_ptr<GeneralData> mGeneralData;
	StateEventManager::ShrPtr mStateEventManagerPtr;
	bool mMusicHasntStarted;

	sf::Sprite m_tutorial_ui_sprite;
	bool m_has_tutorial_displayed;

	bool m_has_tutorial_skipped;
	
};


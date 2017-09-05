#pragma once
#include "StateStack.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>



class GameOverState :	public State{
public:
	GameOverState(StateStack& stack, Context context, const std::string& message);
	virtual ~GameOverState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

private:
	sf::Text mText;
	sf::Font mFont;

	sf::Text mDescText;

	
};


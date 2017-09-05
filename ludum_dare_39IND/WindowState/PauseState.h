#pragma once
#include "StateStack.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState :public State{
public:
	PauseState(StateStack& stack, Context context);
	virtual ~PauseState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);
private:
	sf::Sprite backgroundSprite;
	sf::Text pausedText;
	sf::Text instructionText;
};


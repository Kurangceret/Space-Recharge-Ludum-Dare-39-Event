#pragma once
#include "StateStack.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class MenuState : public State{
public:
	MenuState(StateStack& stack, Context context);
	~MenuState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

	void updateOptionText();
private:
	enum{
		Play,
		Exit
	};
private:
	std::size_t mOptionIndex;
	std::vector<sf::Text> mTexts;

	sf::Sprite m_logo_sprite;
	sf::Texture m_logo_texture;
};


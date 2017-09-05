#pragma once
#include "Component.h"
#include <SFML/Graphics/Text.hpp>


class ComponentArranger;

class TextDisplayComponent :	public Component{
public:
	typedef std::unique_ptr<TextDisplayComponent> Ptr;
	friend ComponentArranger;
public:
	TextDisplayComponent(Entity* entity);
	virtual ~TextDisplayComponent();

	sf::Text& getText();
	void setString(const std::string& str);
	void incrementString(const std::string& str);
private:
	sf::Text mText;
};


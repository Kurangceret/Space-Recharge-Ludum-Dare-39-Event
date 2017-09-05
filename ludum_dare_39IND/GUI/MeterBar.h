#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML\Graphics\Color.hpp>

class EventBase;

class MeterBar{
public:
	MeterBar(const sf::Color& barColor);
	~MeterBar();


	void update(float curValue, float maxValue, sf::Time dt);

	void setSize(const sf::Vector2f& size);
	void setSize(float width, float height);

	void drawMeter(sf::RenderTarget& target, sf::RenderStates states);

private:
	sf::RectangleShape mOuterRect;
	sf::RectangleShape mFillRect;

	sf::Font mFont;
	sf::Text mText;
};


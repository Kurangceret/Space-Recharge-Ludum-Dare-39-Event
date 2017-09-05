#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class EventBase;

class CircleBar{
public:
	CircleBar(const sf::Color& color);
	~CircleBar();

	void update(std::size_t circleNum, sf::Time dt);
	void drawCircles(sf::RenderTarget& target, sf::RenderStates states);
private:
	void addNewCircle();
private:
	std::vector<sf::CircleShape> mCircleList;
	sf::Color mColor;
};


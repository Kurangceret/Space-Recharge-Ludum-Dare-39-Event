#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Body2dComponent;

class SteeringBehavior{
public:
	SteeringBehavior();
	~SteeringBehavior();

	

	

	static sf::Vector2f seekTarget(Body2dComponent* body2dComp,
		const sf::Vector2f& initialPos, const sf::Vector2f& targetPos, sf::Time dt);

	static sf::Vector2f seek(Body2dComponent* body2dComp,
		const sf::Vector2f& direction, sf::Time dt);

private:
	
};


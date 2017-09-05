#include "SteeringBehavior.h"
#include "../Body2dComponent.h"
#include "../../Helper/Math.h"


SteeringBehavior::SteeringBehavior()
{
}


SteeringBehavior::~SteeringBehavior()
{
}


sf::Vector2f SteeringBehavior::seekTarget(Body2dComponent* body2dComp,
	const sf::Vector2f& initialPos, const sf::Vector2f& targetPos, sf::Time dt)
{
	return seek(body2dComp, Math::unitVector(targetPos - initialPos), dt);
}

sf::Vector2f SteeringBehavior::seek(Body2dComponent* body2dComp, const sf::Vector2f& targetDir, sf::Time dt)
{
	sf::Vector2f curVelocity =	body2dComp->getSfLinearUnitVelocity();

	if (curVelocity == sf::Vector2f(0.f, 0.f)) 
		curVelocity = body2dComp->getFacingDir();

	sf::Vector2f targetVelo = targetDir;

	sf::Vector2f steering = targetVelo - curVelocity;
	if (std::abs(steering.y) > 1.f && std::abs(targetVelo.x) < 0.1f)
		steering.x += ((std::abs(steering.y) - 1.f));
	if (std::abs(steering.x) > 1.f && std::abs(targetVelo.y) < 0.1f)
		steering.y += ((std::abs(steering.x) - 1.f));

	steering /= (body2dComp->get_steering_turning_rate() * dt.asSeconds());

	steering += curVelocity;
	


	return steering;
}
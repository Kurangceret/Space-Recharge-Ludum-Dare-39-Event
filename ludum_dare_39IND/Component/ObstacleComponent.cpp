#include "ObstacleComponent.h"
#include "ComponentIdentifier.h"

ObstacleComponent::ObstacleComponent(Entity* entity)
:Component(entity, ComponentIdentifier::ObstacleComponent),
mIsDestroyer(false)
{
}


ObstacleComponent::~ObstacleComponent()
{
}


bool ObstacleComponent::isDestroyer() const
{
	return mIsDestroyer;
}

std::size_t ObstacleComponent::getObstacleCategory() const
{
	return m_obstacle_category;
}
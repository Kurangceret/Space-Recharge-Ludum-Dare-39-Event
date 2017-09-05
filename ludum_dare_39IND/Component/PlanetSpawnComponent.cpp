#include "PlanetSpawnComponent.h"
#include "ComponentIdentifier.h"

PlanetSpawnComponent::PlanetSpawnComponent(Entity* entity)
:Component(entity, ComponentIdentifier::PlanetSpawnComponent)
{
}


PlanetSpawnComponent::~PlanetSpawnComponent()
{
}

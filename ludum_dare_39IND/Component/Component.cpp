#include "Component.h"
#include "ComponentIdentifier.h"
#include <assert.h>

Component::Component(Entity* entity)
:mOwnerEntity(entity),
mIdentifier(ComponentIdentifier::EmptyComponent)
{
	//mComponentIdentifier = ComponentIdentifier::EmptyComponent;
	assert(entity != nullptr);
}

Component::Component(Entity* entity, std::size_t getCompIdentifier)
:mOwnerEntity(entity),
mIdentifier(getCompIdentifier)
{
	assert(entity != nullptr);
}

Component::~Component()
{
}

Entity* Component::getOwnerEntity() const
{
	return mOwnerEntity;
}

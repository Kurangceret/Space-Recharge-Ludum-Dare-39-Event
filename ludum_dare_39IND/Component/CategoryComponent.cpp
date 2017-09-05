#include "CategoryComponent.h"
#include "ComponentIdentifier.h"

CategoryComponent::CategoryComponent(Entity* entity)
:Component(entity, ComponentIdentifier::CategoryComponent),
mCategory(0)
{
}


CategoryComponent::~CategoryComponent()
{
}

std::size_t CategoryComponent::getCategory() const
{
	return mCategory;
}

void CategoryComponent::setCategory(std::size_t category)
{
	mCategory = category;
}

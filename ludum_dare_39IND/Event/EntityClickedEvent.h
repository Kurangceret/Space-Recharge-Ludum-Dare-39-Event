#pragma once
#include "Event.h"

class Entity;
class EntityClickedEvent :
	public Event<EntityClickedEvent>
{
public:
	EntityClickedEvent();
	EntityClickedEvent(Entity* clickedEntity);
	virtual ~EntityClickedEvent();

	Entity* mClickedEntity;
};


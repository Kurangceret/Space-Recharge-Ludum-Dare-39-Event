#include "EntityClickedEvent.h"


EntityClickedEvent::EntityClickedEvent()
:mClickedEntity(nullptr)
{
}


EntityClickedEvent::EntityClickedEvent(Entity* clickedEntity)
:mClickedEntity(clickedEntity){}


EntityClickedEvent::~EntityClickedEvent()
{
}

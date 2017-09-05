#pragma once
#include "Event.h"

class Entity;
class FixtureLogic;

class EntityCollidesEntityEvent :	public Event<EntityCollidesEntityEvent>{
public:
	EntityCollidesEntityEvent();
	virtual ~EntityCollidesEntityEvent();

	Entity* mEntityA;
	Entity* mEntityB;
	const FixtureLogic* mFixtureLogicA;
	const FixtureLogic* mFixtureLogicB;
};


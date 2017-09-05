#pragma once
#include "Event.h"

class Entity;
class FixtureLogic;

class PlayerCollidesSolidEntityEvent : public Event<PlayerCollidesSolidEntityEvent>{
public:
	PlayerCollidesSolidEntityEvent();
	virtual ~PlayerCollidesSolidEntityEvent();

	Entity* mCollidedEntity;
	const FixtureLogic* mPlayerFixtureLogic;
	const FixtureLogic* mCollidedFixtureLogic;
};


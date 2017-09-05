#pragma once
#include "SingleEntitySystem.h"


class AutoBody2dMovementComponent;
class PlayerSystem :	public SingleEntitySystem<PlayerSystem>{
public:
	PlayerSystem();
	virtual ~PlayerSystem();
public:
	void handleEvent(const sf::RenderWindow& renderWindow, 
		const sf::Event& event) override;
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt);

private:
	
private:
	Entity* mPlayerEntity;
	
};


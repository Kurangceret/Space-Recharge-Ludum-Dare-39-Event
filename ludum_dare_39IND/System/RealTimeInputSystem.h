#pragma once
#include "SingleEntitySystem.h"

class PlayerComponent;
class RealTimeInputComponent;

class RealTimeInputSystem :	public SingleEntitySystem<RealTimeInputSystem>{
public:
	RealTimeInputSystem();
	virtual ~RealTimeInputSystem();
	void handleEvent(const sf::RenderWindow& renderWindow, const sf::Event&) override;
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
	bool initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
private:
	std::size_t mMaxInputCount;
	std::size_t mMaxPressedCount;

	RealTimeInputComponent* mPlayerInputComp;
	bool mNextFrameSetAllDoubleTap;

	sf::Keyboard::Key mCurKey;
	sf::Time mMaxWaitTimeDoubleTap;
	sf::Time mCurWaitTimeDoubleTap;
};


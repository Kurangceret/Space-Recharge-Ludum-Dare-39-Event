#pragma once
#include "Component.h"

#include <functional>
#include <SFML/System/Time.hpp>
#include <queue>

class Entity;
class DamageSystem;

struct DamageData{
	DamageData(Entity* damagerEntity, Entity* damagedEntity, 
		float damage, std::size_t damageType, std::size_t damagedEntityDefType);
	Entity* mDamagerEntity;
	Entity* mDamagedEntity;
	float mDamage;
	std::size_t mDamageType;
	std::size_t mDamagedEntityDefType;
};


class HealthComponent :	public Component{
public:
	friend class DamageSystem;
	typedef std::function<void(Entity*, float&)> DamagedReactor;
public:
	HealthComponent(Entity* entity, DamagedReactor damagedReactor = DamagedReactor());
	virtual ~HealthComponent();

	void increaseHealth(float health);
	void setMaxHealth(float health);
	void setCurrentHealth(float health);
	
	bool hasZeroHealth() const;
	float getMaxHealth() const;
	float getCurrentHealth() const;

	void damage(Entity* damagerEntity, float damage, std::size_t damageType);

	//calling this function once will set the member variable to reset back
	bool wasHealthChanged();

	void setDamagedReactor(const DamagedReactor& damagedReactor);
	
	//void setImmuneTimer(const sf::Time& timer);
	void setIsImmune(bool flag);
	bool isImmune() const;
	void update(sf::Time dt);

	void doDirectDamage(Entity* damageSender, float damage);
private:
	
private:
	float mMaxHealth;
	float mCurrentHealth;

	bool mWasHealthChanged;
	DamagedReactor mDamagedFunc;

	std::queue<DamageData> mDamageQueue;
	//sf::Time mImmuneTimer;
	bool mIsImmune;
};


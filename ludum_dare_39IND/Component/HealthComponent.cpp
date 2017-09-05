#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "../Entity/Entity.h"

/*#include "EntityGotDamagedEvent.h"
#include "EventManager.h"*/
#include "ComponentIdentifier.h"

DamageData::DamageData(Entity* damagerEntity, Entity* damagedEntity,
	float damage, std::size_t damageType, std::size_t damagedEntityDefType)
:mDamagerEntity(damagerEntity),
mDamagedEntity(damagedEntity),
mDamage(damage),
mDamageType(damageType),
mDamagedEntityDefType(damagedEntityDefType)
{}


HealthComponent::HealthComponent(Entity* entity, DamagedReactor damagedReactor)
:Component(entity, ComponentIdentifier::HealthComponent),
mDamagedFunc(damagedReactor),
mMaxHealth(1),
mCurrentHealth(1),
mWasHealthChanged(false),
mIsImmune(false)
//mImmuneTimer(sf::Time::Zero)
{
	mDamagedFunc = [&](Entity*, float&){

	};
}


HealthComponent::~HealthComponent()
{
}


void HealthComponent::increaseHealth(float health)
{
	mCurrentHealth += std::abs(health);
	mWasHealthChanged = true;
}

void HealthComponent::setMaxHealth(float health)
{
	mMaxHealth = health;
	mWasHealthChanged = true;
}

void HealthComponent::setCurrentHealth(float health)
{
	mCurrentHealth = health;
	mWasHealthChanged = true;
}

bool HealthComponent::hasZeroHealth() const
{
	return mCurrentHealth <= 0.f;
}

float HealthComponent::getMaxHealth() const
{
	return mMaxHealth;
}

float HealthComponent::getCurrentHealth() const
{
	return mCurrentHealth;
}

void HealthComponent::damage(Entity* damagerEntity, float damage, std::size_t damageType)
{
	std::size_t defenseType = 0;
	
	mDamageQueue.push(DamageData(damagerEntity, getOwnerEntity(), damage, damageType, defenseType));
}

bool HealthComponent::wasHealthChanged()
{
	bool original = mWasHealthChanged;
	mWasHealthChanged = false;
	return original;
}

void HealthComponent::setDamagedReactor(const DamagedReactor& damagedReactor)
{
	mDamagedFunc = damagedReactor;
}

/*void HealthComponent::setImmuneTimer(const sf::Time& timer)
{
	mImmuneTimer = timer;
}*/

void HealthComponent::setIsImmune(bool flag)
{
	mIsImmune = flag;
}

bool HealthComponent::isImmune() const
{
	//return mImmuneTimer > sf::Time::Zero;
	return mIsImmune;
}

void HealthComponent::update(sf::Time dt)
{
	
}

void HealthComponent::doDirectDamage(Entity* damageSender, float damage)
{
	if (isImmune())
		damage = 0.f;

	mDamagedFunc(damageSender, damage);
	mCurrentHealth -= std::abs(damage);
	mWasHealthChanged = true;
}
#include "PlayerComponent.h"
#include "ComponentIdentifier.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include "../Helper/Constant.h"
#include "Body2dComponent.h"
#include "../Entity/Entity.h"
#include "../Component/SpriteComponent.h"

PlayerComponent::PlayerComponent(Entity* entity)
:Component(entity, ComponentIdentifier::PlayerComponent),
mCurStateEventName(""),
m_is_player_available(true),
m_is_player_in_charge_area(false)
{
}

PlayerComponent::~PlayerComponent()
{
}


void PlayerComponent::setCurStateEventName(const std::string& stateEventName)
{
	mCurStateEventName = stateEventName;
}

std::string PlayerComponent::getCurStateEventName() const
{
	return mCurStateEventName;
}


bool PlayerComponent::is_player_available() const
{
	return m_is_player_available;
}

void PlayerComponent::set_is_player_in_charge_area(bool flag)
{
	m_is_player_in_charge_area = flag;
}

void PlayerComponent::increase_drain_power(float val)
{
	getOwnerEntity()->getComp<SpriteComponent>()->beganBlink();
	m_cur_drain_power_rate += std::abs(val);
	if (m_cur_drain_power_rate > m_max_drain_power_rate)
		m_cur_drain_power_rate = m_max_drain_power_rate;
}
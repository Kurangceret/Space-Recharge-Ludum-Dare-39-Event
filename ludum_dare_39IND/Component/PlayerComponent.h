#pragma once
#include "Component.h"

#include <string>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <SFML/System/Time.hpp>
#include <vector>

class PlayerSystem;
class ComponentArranger;
class StandardStateEvent;

class PlayerComponent :	public Component{
public:
	friend class ComponentArranger;
	friend class PlayerSystem;
	friend class StandardStateEvent;

public:
	PlayerComponent(Entity* entity);
	virtual ~PlayerComponent();
	
	
	void setCurStateEventName(const std::string& stateEventName);
	std::string getCurStateEventName() const;
	
	bool is_player_available() const;
		
	void set_is_player_in_charge_area(bool flag);

	void increase_drain_power(float val);
private:
	

	std::string mCurStateEventName;
	std::vector<std::size_t> mDamagedEntities;

	bool m_is_player_available;
	bool m_is_player_in_charge_area;

	float m_max_power;
	float m_cur_power;

	float m_min_drain_power_rate;
	float m_cur_drain_power_rate;
	float m_max_drain_power_rate;
};


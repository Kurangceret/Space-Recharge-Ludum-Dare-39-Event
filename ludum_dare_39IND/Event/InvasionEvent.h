#pragma once
#include "Event.h"
#include <SFML/System/Time.hpp>
class InvasionEvent :	public Event<InvasionEvent>{
public:
	InvasionEvent(const sf::Time& invasion_time);
	virtual ~InvasionEvent();

	sf::Time m_cur_invasion_time;
};


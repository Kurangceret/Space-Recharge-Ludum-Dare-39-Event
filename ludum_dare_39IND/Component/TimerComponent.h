#pragma once
#include "Component.h"
#include <SFML/System/Time.hpp>
#include <unordered_map>
#include <string>

struct TimerData{
	TimerData();
	sf::Time mElapsedTime;
	sf::Time mTimeLimit;
};

class ComponentArranger;

class TimerComponent :	public Component{
public:
	friend ComponentArranger;
public:
	TimerComponent(Entity* entity);
	virtual ~TimerComponent();
	
	void updateEntireTimers(float seconds);
	void updateTimer(const std::string& timerName, float seconds);
	void setNewTimeLimitToTimer(const std::string& timerName, float newSeconds);
	sf::Time getTimeLimitTimer(const std::string& timerName) const;
	bool isTimerElapsed(const std::string& timerName) const;

private:
	std::unordered_map<std::string, TimerData> mTimerDataList;
};


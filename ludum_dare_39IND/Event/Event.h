#pragma once
#include "BaseEvent.h"

class EventManager;

template <class Derived>
class Event :public BaseEvent{
public:
	typedef std::unique_ptr<Event> Ptr;
public:
	virtual ~Event() = default;

protected:
	Event() = default;
private:
	friend EventManager;
	static RefID getRefID(){
		static RefID refId = mRefIDCounter++;
		return refId;
	}
};



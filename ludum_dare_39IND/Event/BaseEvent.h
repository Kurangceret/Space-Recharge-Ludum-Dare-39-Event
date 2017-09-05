#pragma once
#include <cstdint>
#include <memory>

class BaseEvent{
public:
	typedef std::size_t RefID;
	typedef std::unique_ptr<BaseEvent> Ptr;
public:
	virtual ~BaseEvent();
protected:
	BaseEvent();
protected:
	static RefID mRefIDCounter;
};


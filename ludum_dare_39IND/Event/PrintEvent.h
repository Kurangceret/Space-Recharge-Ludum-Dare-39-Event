#pragma once
#include "Event.h"
class PrintEvent :	public Event<PrintEvent>{
public:
	PrintEvent(int a);
	virtual ~PrintEvent();

	int mRandomInteger;
};


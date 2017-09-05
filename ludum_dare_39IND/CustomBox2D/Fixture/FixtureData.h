#pragma once
#include "BaseFixtureData.h"

template<class T>
class FixtureData :	public BaseFixtureData{
public:
	virtual ~FixtureData() = default;

	static RefID getRefID(){
		static RefID counter = mRefIDCounter++;
		return counter;
	}
protected:
	FixtureData() = default;
};


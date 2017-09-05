#pragma once
#include <cstdint>
#include <memory>

class FixtureLogic;

class BaseFixtureData{
public:
	typedef std::size_t RefID;
	typedef std::unique_ptr<BaseFixtureData> Ptr;
	friend class FixtureLogic;
public:
	
	virtual ~BaseFixtureData();
protected:
	BaseFixtureData();
protected:
	static RefID mRefIDCounter;
};


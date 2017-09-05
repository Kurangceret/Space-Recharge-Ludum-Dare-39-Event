#pragma once
#include "FixtureData.h"
#include <SFML/System/Vector2.hpp>

class TileSideFixtureData :	public FixtureData<TileSideFixtureData>{
public:
	typedef std::unique_ptr<TileSideFixtureData> Ptr;
public:
	TileSideFixtureData();
	virtual ~TileSideFixtureData();

	//rectangle aiming dir generated from tile sides
	sf::Vector2f mSensorDir;
	//theta is the 90 degree sides of a right triangle
	sf::Vector2f mDirToThetaDegree;

	bool mAllowGravityLimiter;
};


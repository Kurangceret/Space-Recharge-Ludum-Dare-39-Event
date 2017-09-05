#pragma once
#include "FixtureData.h"
#include <SFML/System/Vector2.hpp>

class PolygonFixtureData :	public FixtureData<PolygonFixtureData>{
public:
	typedef std::unique_ptr<PolygonFixtureData> Ptr;
public:
	PolygonFixtureData();
	virtual ~PolygonFixtureData();

	sf::Vector2f mPolygonPos;
};


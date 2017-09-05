#pragma once
#include <string>

class Body2dComponent;
struct lua_State;


class TileSidesSensorGenerator{
public:
	TileSidesSensorGenerator();
	~TileSidesSensorGenerator();

	void generateTileSensorSidesScript(Body2dComponent* body2dComp, 
		const std::string& dirToThetaStr,
		bool allowGravityLimiter,
		lua_State* luaState);

	void generatePolygonFixtureData(Body2dComponent* body2dComp, 
		const std::string& fixtureName, 
		float xPos, 
		float yPos);
private:
};


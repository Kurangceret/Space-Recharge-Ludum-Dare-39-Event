#include "TileSidesSensorGenerator.h"
#include "../Component/Body2dComponent.h"
#include "../Entity/Entity.h"
#include "../Helper/Math.h"
#include "../Helper/Utility.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include <vector>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <LuaBridge.h>
#include "../CustomBox2D/Fixture/TileSideFixtureData.h"
#include <iostream>
#include <cstring>
#include "../CustomBox2D/Fixture/PolygonFixtureData.h"


extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};

TileSidesSensorGenerator::TileSidesSensorGenerator()
{
}


TileSidesSensorGenerator::~TileSidesSensorGenerator()
{
}


void TileSidesSensorGenerator::generateTileSensorSidesScript(Body2dComponent* body2dComp,
	const std::string& dirToThetaStr, bool allowGravityLimiter, lua_State* luaState)
{
	luabridge::LuaRef verticesTable = luabridge::LuaRef::fromStack(luaState, 5);
	std::size_t verticeIndex = 1;

	std::vector<sf::Vector2f> pixelVList;
	//sf::Vector2f prevPixelVertPos;
	
	const float sensorThickness = 3.f;
	const float sensorAddGap = 0.f;

	const auto& bodyFilterData = body2dComp->getMainFixture()->GetFilterData();
	std::size_t categoryToBeSolved = body2dComp->getMainFixtureLogic().mCategoryToBeCollisionSolved;

	while (!verticesTable[verticeIndex].isNil()){
		luabridge::LuaRef firstVertT = verticesTable[verticeIndex].cast<luabridge::LuaRef>();
		sf::Vector2f firstVertPos(firstVertT[1].cast<float>(), firstVertT[2].cast<float>());

		luabridge::LuaRef secondVertT = verticesTable[verticeIndex + 1].isNil() ? 
			verticesTable[1] : verticesTable[verticeIndex + 1];
		
		sf::Vector2f secondVertPos(secondVertT[1].cast<float>(), secondVertT[2].cast<float>());

		sf::Vector2f diffTwoVert = secondVertPos - firstVertPos;
		float sensorLength = Math::vectorLength(diffTwoVert);
		sf::Vector2f sensorDir = Math::unitVector(diffTwoVert, sensorLength);
		sf::Vector2f revSensorDir = sensorDir * -1.f;

		if (dirToThetaStr != "" && (sensorDir.x == 0.f || sensorDir.y == 0.f)){
			verticeIndex++;
			continue;
		}

		sf::Vector2f sensorCrossDirPos = Math::degreeToVector(Math::vectorToDegree(sensorDir, false) + 90.f);
		sf::Vector2f sensorCrossDirNeg = sensorCrossDirPos * -1.f;

		std::vector<b2Vec2> verticesList;
		verticesList.resize(4);

		sf::Vector2f firstCornerPos = firstVertPos + (sensorCrossDirPos * sensorThickness);
		sf::Vector2f thirdCornerPos = secondVertPos + (sensorCrossDirNeg * sensorThickness);

		verticesList[0] = Utility::formatVecPixelToMeter(firstCornerPos + (revSensorDir * sensorAddGap));
		verticesList[1] = Utility::formatVecPixelToMeter(secondVertPos + 
			(sensorCrossDirPos * sensorThickness) + (sensorDir * sensorAddGap));
		verticesList[2] = Utility::formatVecPixelToMeter(thirdCornerPos + (sensorDir * sensorAddGap));
		verticesList[3] = Utility::formatVecPixelToMeter(firstVertPos + 
			(sensorCrossDirNeg * sensorThickness) + (revSensorDir * sensorAddGap));

		//determining middle pos of a polygon
		PolygonFixtureData::Ptr polygonFixtureData(new PolygonFixtureData());
		polygonFixtureData->mPolygonPos = sf::Vector2f();
		if (sensorDir.x == 0.f || sensorDir.y == 0.f){
			float lengthFrom3PosTo1Pos = Math::vectorLength(thirdCornerPos - firstCornerPos);
			sf::Vector2f dirToThirdCorner = Math::unitVector(
				thirdCornerPos - firstCornerPos, lengthFrom3PosTo1Pos);

			polygonFixtureData->mPolygonPos =
				firstCornerPos + (dirToThirdCorner * (lengthFrom3PosTo1Pos / 2.f));
		}
		//

		body2dComp->mFixtureLogicList.push_back(FixtureLogic());

		FixtureLogic& fixtureLogic = body2dComp->mFixtureLogicList.back();
		fixtureLogic.mFixtureName = "SideSensor" + std::to_string(verticeIndex);
		fixtureLogic.mIsActive = true;
		fixtureLogic.mIsInteractionFixture = false;
		fixtureLogic.mCategoryToBeCollisionSolved = categoryToBeSolved;
		fixtureLogic.mDrawColor = sf::Color(155, 0, 0, 255);
		fixtureLogic.mFixtureDataList.insert(std::make_pair(FixtureData<PolygonFixtureData>::getRefID(), 
			std::move(polygonFixtureData)));


		b2PolygonShape polygonShape;
		polygonShape.Set(&(*verticesList.begin()), verticesList.size());

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.density = 0.0;
		fixtureDef.friction = 0.0;
		fixtureDef.restitution = 0.0;
		fixtureDef.filter = bodyFilterData;
		fixtureDef.userData = &fixtureLogic;
		fixtureDef.shape = &polygonShape;
		
		b2Fixture* fixture = body2dComp->mBody->CreateFixture(&fixtureDef);
		fixtureLogic.mFixture = fixture;
		
		TileSideFixtureData::Ptr tileSideData(new TileSideFixtureData());
		tileSideData->mDirToThetaDegree = sf::Vector2f();
		tileSideData->mAllowGravityLimiter = allowGravityLimiter;
		//tileSideData->mAllowGravityLimiter = false;
		if (dirToThetaStr != "" && sensorDir.x != 0.f && sensorDir.y != 0.f){

			const char* delimiters = ",";
			char newStr[8];
			char* ch;
			
			std::strcpy(newStr,	dirToThetaStr.c_str());

			sf::Vector2f dirToTheta;
			int i = 0;
			ch = strtok(newStr, delimiters);
			while (ch != NULL){
				if (i == 0) dirToTheta.x = std::atoi(ch);
				else if (i == 1) dirToTheta.y = std::atoi(ch);
				ch = strtok(NULL, delimiters);
				i++;
			} 
			tileSideData->mDirToThetaDegree = dirToTheta;
			
		}
		tileSideData->mSensorDir = sensorDir;
		fixtureLogic.mFixtureDataList.insert(std::make_pair(FixtureData<TileSideFixtureData>::getRefID(), 
			std::move(tileSideData)));

		verticeIndex++;
	}
}



void TileSidesSensorGenerator::generatePolygonFixtureData(Body2dComponent* body2dComp,
	const std::string& fixtureName,
	float xPos,
	float yPos)
{
	PolygonFixtureData::Ptr polygonFixtureData(new PolygonFixtureData());
	polygonFixtureData->mPolygonPos.x = xPos;
	polygonFixtureData->mPolygonPos.y = yPos;

	body2dComp->addFixtureData(FixtureData<PolygonFixtureData>::getRefID(), 
		std::move(polygonFixtureData), fixtureName);
}
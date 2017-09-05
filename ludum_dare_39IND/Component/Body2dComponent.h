#pragma once
#include "Component.h"
#include <Box2D\Common\b2Settings.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Collision\Shapes\b2Shape.h>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <vector>
#include <unordered_map>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

#include "../CustomBox2D/Fixture/FixtureData.h"
class b2PolygonShape;
class b2CircleShape;

class TileSidesSensorGenerator;
class b2Body;
class b2Fixture;
class ComponentArranger;
class EntityManager;
class b2World;
class RenderingSpriteSystem;

struct lua_State;

typedef std::unique_ptr<luabridge::LuaRef> LuaRefPtr;

class Body2dComponent;

class FixtureLogic{
public:
	friend class TileSidesSensorGenerator;
	friend class Body2dComponent;
	typedef std::unordered_map<BaseFixtureData::RefID,
		BaseFixtureData::Ptr> FixtureDataMap;
public:
	FixtureLogic();
	FixtureLogic(FixtureLogic& fixtureLogic);

	uint16 getCategory() const;

	b2Fixture *mFixture;
	std::string mFixtureName;

	LuaRefPtr mInteractionLuaFunc;
	LuaRefPtr mBeginContactLuaFunc;
	LuaRefPtr mEndContactLuaFunc;
	LuaRefPtr mPreSolveLuaFunc;
	LuaRefPtr mPostSolveLuaFunc;

	std::size_t mCategoryToBeCollisionSolved;


	std::size_t mOriCategoryToBeCollisionSolved;
	std::size_t mOriCategoryToBeCollided;

	bool mIsActive;

	sf::Color mDrawColor;
	//
	bool mIsInteractionFixture;

	template <class F>
	const F* getFixtureData() const;
private:
	FixtureDataMap mFixtureDataList;
};


template <class F>
const F* FixtureLogic::getFixtureData() const
{
	BaseFixtureData::RefID refId = FixtureData<F>::getRefID();
	
	/*auto findIter = std::find_if(mFixtureDataList.begin(), mFixtureDataList.end(), 
		[&](const BaseFixtureData::Ptr& fixtureData) -> bool 
	{
		return fixtureData->mRefIDCounter == refId;
	});*/
	auto findIter = mFixtureDataList.find(refId);

	if (findIter == mFixtureDataList.end())
		return nullptr;

	return static_cast<const F*>(findIter->second.get());
}
struct b2ContactEdge;

//this class is a wrapper to b2Body implmentation
//the reason why b2Body classes were made private is also to
//FOR NOW:
//it is safe to assume that the body component 
//we are using will only contains one fixture with one shapes
//NVM LUL, box2d power is multile fixtures
//:D
class Body2dComponent :	public Component{
public:
	friend class ComponentArranger;
	friend class EntityManager;
	friend class RenderingSpriteSystem;
	friend class TileSidesSensorGenerator;

	static std::string mMainFixtureName;
public:
	Body2dComponent(Entity* entity);
	virtual ~Body2dComponent();
	
	b2ContactEdge* getBodyContactList() const;
	b2World* getb2World() const;

	void setGravityScale(float val);

	void setMainFixtureActive(bool flag);
	bool isMainFixtureActive(bool flag) const;

	void setBodyActive(bool flag);
	bool isBodyActive() const;

	void setBodyAwake(bool flag);
	void move(const sf::Vector2f& pixelMovement);
	//if body has a shape of box
	//its origin is set to be center by default from box2d API
	//so the origin in this classes may not be positive
	sf::Transform getSfDrawTransform() const;

	sf::Vector2f getSfLinearVelocity() const;
	sf::Vector2f getSfLinearUnitVelocity() const;
	

	float32 getMass() const;
	float32 getAngle() const;
	float32 getAngleDegree() const;

	void setLinearVelocity(const b2Vec2& linearVelo);
	b2Vec2 getLinearUnitVelocity() const;
	b2Vec2 getLinearVelocity() const;
	
	void setTransform(const b2Transform& transform);
	void setPosition(float pixelX, float pixelY);
	void setPosition(const sf::Vector2f& pixelPos);
	void setPositionScript(float pixelX, float pixelY);

	void moveVeloPixelX(float pixelVX, bool withSpeed = true);
	void moveVeloPixelY(float pixelVY, bool withSpeed = true);
	void setVelocityX(float pixelVX, bool withSpeed = true);
	void setVelocityY(float pixelVY, bool withSpeed = true);
	void setVelocity(float pixelVX, float pixelVY, bool withSpeed = true);
	void setVelocity(const sf::Vector2f& pixelV, bool withSpeed = true);
	void setVelocityScript(float pixelVX, float pixelVY, bool withSpeed = true);

	void applyLinearPixelImpulseToCenter(const sf::Vector2f& impulse);
	void applyPixelForceToCenter(const sf::Vector2f& force);

	void setMass(float32 mass);
	//box2d uses radian
	void setAngle(float32 radian);
	void rotateAngle(float32 radian);

	void setAngleDegree(float32 degree);
	void rotateAngleDegree(float32 degree);

	void setBaseSpeed(float baseSpeed);
	float getBaseSpeed() const;


	void setFixtureDrawColor(const std::string& fixtureName, const sf::Color& color);

	void setMainFixtureVerticesShapeScript(lua_State* luaState);
	void setFixtureVerticesShape(const std::vector<sf::Vector2f>& pixelVerticesList, const std::string& fixtureName = "");
	void setFixtureVerticesShape(const std::vector<b2Vec2>& meterVerticesList, const std::string& fixtureName = "");

	void setFixtureVerticesShapeScript(const std::string& fixtureName, lua_State* luaState);

	b2Vec2 getPosition(bool withOrigin = false) const;
	sf::Vector2f getSfPosition(bool withOrigin = false) const;
	sf::Vector2f getPositionScript(bool withOrigin = false) const;
	sf::Vector2f getSfWorldPosition(bool withOrigin = false) const;
	b2Vec2 getWorldPosition(bool withOrigin = false) const;
	sf::Vector2f getWorldPositionScript(bool withOrigin = false) const;
	b2Transform getWorldTransform(bool withOrigin = false) const;
	b2Transform getTransform(bool withOrigin = false) const;
	sf::Transform getSfTransform(bool withOrigin = false) const;
	sf::Transform getSfWorldTransform(bool withOrigin = false) const;
	//sf::Vector2f getBoxSize(bool* isBoxType) const;

	b2CircleShape* getMainCircleShape() const;
	b2PolygonShape* getMainPolyShape() const;
	b2EdgeShape* getMainEdgeShape() const;

	void setMainFixtureNewShape(b2Shape* newShape);
	void setFixtureNewShape(b2Shape* newShape, const std::string& fixtureName);

	b2Shape* cloneMainFixtureShape(b2BlockAllocator* blockAllocator) const;
	b2Shape* cloneFixtureShape(const std::string& fixtureName, b2BlockAllocator* blockAllocator) const;

	b2Fixture* getFixture(const std::string& fixtureName) const;
	void setFixtureActive(const std::string& fixtureName, bool flag);

	const FixtureLogic& getFixtureLogic(const std::string& fixtureName) const;
	const FixtureLogic& getConstFixtureLogic(const std::string& fixtureName) const;

	void addFixtureData(BaseFixtureData::RefID refId, 
		BaseFixtureData::Ptr fixtureDataPtr, const std::string& fixtureName);

	sf::Vector2f getFacingDir() const;
	float getFacingDirX() const;
	float getFacingDirY() const;

	float get_steering_turning_rate() const;
private:
	b2Fixture* getMainFixture() const;
	const FixtureLogic& getMainFixtureLogic() const;
	FixtureLogic& getFixtureLogic(const std::string& fixtureName);

	void setNewShape(b2Shape* newShape, FixtureLogic& fixtureLogic);
private:
	b2Body* mBody;
	std::vector<FixtureLogic> mFixtureLogicList;

	int mIndexOfMainFixture;
	//b2Fixture* mBodyShapeFixture;
	sf::Vector2f mBodyShapeOrigin;
	float mBaseSpeed;

	sf::Vector2f mFacingDir;

	bool mRotateBasedOnVelo;

	float m_steering_turning_rate;
};


#include "Body2dComponent.h"
#include "ComponentIdentifier.h"
#include <Box2D\Dynamics\b2Body.h>
#include "../Helper/Constant.h"
#include <Box2D\Common\b2Math.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\b2Fixture.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Collision\Shapes\b2EdgeShape.h>
#include "../Helper/Utility.h"
#include "../Helper/Math.h"

#include "../Entity/Entity.h"
#include "../Component/EntityChildrenComponent.h"
#include "../Component/EntityParentComponent.h"


FixtureLogic::FixtureLogic()
:mFixture(nullptr),
mPreSolveLuaFunc(nullptr),
mPostSolveLuaFunc(nullptr),
mInteractionLuaFunc(nullptr),
mIsInteractionFixture(false),
mIsActive(true),
mDrawColor(128, 128, 128, 255),
mCategoryToBeCollisionSolved(0)
{
}

FixtureLogic::FixtureLogic(FixtureLogic& fixtureLogic)
:mCategoryToBeCollisionSolved(fixtureLogic.mCategoryToBeCollisionSolved),
mPreSolveLuaFunc(fixtureLogic.mPreSolveLuaFunc.release()),
mPostSolveLuaFunc(fixtureLogic.mPostSolveLuaFunc.release()),
mInteractionLuaFunc(fixtureLogic.mInteractionLuaFunc.release()),
mBeginContactLuaFunc(fixtureLogic.mBeginContactLuaFunc.release()),
mEndContactLuaFunc(fixtureLogic.mEndContactLuaFunc.release()),
mFixture(fixtureLogic.mFixture),
mIsInteractionFixture(fixtureLogic.mIsInteractionFixture),
mIsActive(fixtureLogic.mIsActive),
mDrawColor(fixtureLogic.mDrawColor)
{
	for (auto& baseDataPtr : fixtureLogic.mFixtureDataList)
		mFixtureDataList.insert(std::make_pair(baseDataPtr.first, std::move(baseDataPtr.second)));
}

uint16 FixtureLogic::getCategory() const
{
	return mFixture->GetFilterData().categoryBits;
}

std::string Body2dComponent::mMainFixtureName = "MainFixture";

Body2dComponent::Body2dComponent(Entity* entity)
:Component(entity, ComponentIdentifier::Body2dComponent),
mBaseSpeed(0.f),
mIndexOfMainFixture(-1),
mFacingDir(1.f, 0.f),
m_steering_turning_rate(500.f)
{
	
}


Body2dComponent::~Body2dComponent()
{
}

b2ContactEdge* Body2dComponent::getBodyContactList() const
{
	return mBody->GetContactList();
}

b2World* Body2dComponent::getb2World() const
{
	return mBody->GetWorld();
}

void Body2dComponent::setBodyActive(bool flag)
{
	mBody->SetActive(flag);
}

bool Body2dComponent::isBodyActive() const
{
	return mBody->IsActive();
}

sf::Transform Body2dComponent::getSfDrawTransform() const
{
	sf::Transform transform = getSfWorldTransform();
	return transform;
}

sf::Vector2f Body2dComponent::getSfLinearUnitVelocity() const
{
	return Math::unitVector(getSfLinearVelocity());
}

sf::Vector2f Body2dComponent::getSfLinearVelocity() const
{
	b2Vec2 b2Vec = mBody->GetLinearVelocity();
	return sf::Vector2f(Utility::formatMeterToPixel(b2Vec.x), Utility::formatMeterToPixel(b2Vec.y));
}


sf::Vector2f Body2dComponent::getSfWorldPosition(bool withOrigin) const
{
	return Utility::formatVecMeterToPixel(getWorldPosition(withOrigin));
}

b2Vec2 Body2dComponent::getWorldPosition(bool withOrigin) const
{
	return getWorldTransform(withOrigin).p;
}

sf::Vector2f Body2dComponent::getWorldPositionScript(bool withOrigin) const
{
	return getSfWorldPosition(withOrigin);
}

b2Transform Body2dComponent::getWorldTransform(bool withOrigin) const
{
	b2Transform finalTransform = getTransform(withOrigin);
	EntityChildrenComponent* childComp = getOwnerEntity()->nonCreateComp<EntityChildrenComponent>();
	

	if (!childComp)
		return finalTransform;
		
	/*finalTransform.p += Utility::formatVecPixelToMeter(childComp->getPosRelativeToParent());
	finalTransform.q.Set(Utility::toRadian(Utility::vectorToDegree(childComp->getFacingDirWithParent())));
	finalTransform.p += childComp->getParent()->getComp<Body2dComponent>()->getWorldTransform(withOrigin).p;*/
	Entity* parentEntity = childComp->getParent();

	if (!parentEntity)
		return finalTransform;

	Body2dComponent* parentBodyComp = parentEntity->nonCreateComp<Body2dComponent>();
	if (!parentBodyComp)
		return finalTransform;

	b2Transform parentTransform = parentBodyComp->getWorldTransform(withOrigin);
	finalTransform.p += parentTransform.p;

	//const EntityChildData& entityChildData = 
		//parentEntity->getComp<EntityParentComponent>()->getChildData(mOwnerEntity);

	//if (entityChildData.mAffectedByParentRotation)
	//finalTransform.q.Set(finalTransform.q.GetAngle() + parentTransform.q.GetAngle());
		

	return finalTransform;
}

b2Transform Body2dComponent::getTransform(bool withOrigin) const
{
	b2Transform newTransform = mBody->GetTransform();
	if (withOrigin) 
		newTransform.p += Utility::formatVecPixelToMeter(mBodyShapeOrigin);
	return newTransform;
}

sf::Transform Body2dComponent::getSfTransform(bool withOrigin) const
{
	return Utility::formatb2TransformToSfTransform(getTransform(withOrigin));
}

sf::Transform Body2dComponent::getSfWorldTransform(bool withOrigin) const
{
	return Utility::formatb2TransformToSfTransform(getWorldTransform(withOrigin));
}

sf::Vector2f Body2dComponent::getSfPosition(bool withOrigin) const
{
	return Utility::formatVecMeterToPixel(getPosition(withOrigin));
}

sf::Vector2f Body2dComponent::getPositionScript(bool withOrigin) const
{
	return getSfPosition(withOrigin);
}

float32 Body2dComponent::getMass() const
{
	return mBody->GetMass();
}

float32 Body2dComponent::getAngle() const
{
	return mBody->GetAngle();
}

float32 Body2dComponent::getAngleDegree() const
{
	return Math::toDegree(mBody->GetAngle());
}

b2Vec2 Body2dComponent::getLinearVelocity() const
{
	return mBody->GetLinearVelocity();
}

b2Vec2 Body2dComponent::getPosition(bool withOrigin) const
{
	b2Vec2 finalPos = mBody->GetPosition();
	if (withOrigin)
		finalPos += Utility::formatVecPixelToMeter(mBodyShapeOrigin);

	return finalPos;
}

void Body2dComponent::setLinearVelocity(const b2Vec2& linearVelo)
{
	if (mRotateBasedOnVelo){
		if (linearVelo.x != 0.f)
			mFacingDir.x = linearVelo.x ;
		if (linearVelo.y != 0.f)
			mFacingDir.y = linearVelo.y;
		float angleDegree = Math::vectorToDegree(mFacingDir);
		setAngleDegree(std::ceil(angleDegree));
	}
	
	mBody->SetLinearVelocity(linearVelo);
}

b2Vec2 Body2dComponent::getLinearUnitVelocity() const
{
	b2Vec2 linearVelo = getLinearVelocity();
	if (linearVelo == b2Vec2(0.f, 0.f))
		return linearVelo;

	linearVelo *= (1.0f / linearVelo.Length());
	return linearVelo;
}

void Body2dComponent::setBaseSpeed(float baseSpeed)
{
	mBaseSpeed = baseSpeed;
}


float Body2dComponent::getBaseSpeed() const
{
	return mBaseSpeed;
}

void Body2dComponent::setTransform(const b2Transform& transform)
{
	mBody->SetTransform(transform.p, transform.q.GetAngle());
}

void Body2dComponent::setPosition(float pixelX, float pixelY)
{
	mBody->SetTransform(b2Vec2(Utility::formatPixelToMeter(pixelX), 
		Utility::formatPixelToMeter(pixelY)), mBody->GetAngle());
}

void Body2dComponent::setPosition(const sf::Vector2f& pixelPos)
{
	setPosition(pixelPos.x, pixelPos.y);
}

void Body2dComponent::setPositionScript(float pixelX, float pixelY)
{
	setPosition(pixelX, pixelY);
}

void Body2dComponent::moveVeloPixelX(float pixelVX, bool withSpeed)
{
	if (withSpeed){
		pixelVX *= mBaseSpeed;
	}
	float oriVX = getSfLinearVelocity().x;
	setVelocityX(oriVX + pixelVX, false);
}

void Body2dComponent::moveVeloPixelY(float pixelVY, bool withSpeed)
{
	if(withSpeed){
		pixelVY *= mBaseSpeed;
	}
	float oriVY = getSfLinearVelocity().y;
	setVelocityY(oriVY + pixelVY, false);
}

void Body2dComponent::setVelocityX(float pixelVX, bool withSpeed)
{
	b2Vec2 originalVec2 = mBody->GetLinearVelocity();
	if (withSpeed){
		pixelVX *= mBaseSpeed;
	}
	originalVec2.x = Utility::formatPixelToMeter(pixelVX);
	setLinearVelocity(originalVec2);
	//mBody->SetLinearVelocity(originalVec2);
}

void Body2dComponent::setVelocityY(float pixelVY, bool withSpeed)
{
	b2Vec2 originalVec2 = mBody->GetLinearVelocity();
	if (withSpeed){
		pixelVY *= mBaseSpeed;
	}
	originalVec2.y = Utility::formatPixelToMeter(pixelVY);
	setLinearVelocity(originalVec2);
	//mBody->SetLinearVelocity(originalVec2);
}

void Body2dComponent::setVelocity(float pixelVX, float pixelVY, bool withSpeed)
{
	b2Vec2 vec2(pixelVX, pixelVY);
	if (withSpeed){
		vec2 *= mBaseSpeed;
	}
	vec2.x = Utility::formatPixelToMeter(vec2.x);
	vec2.y = Utility::formatPixelToMeter(vec2.y);

	setLinearVelocity(vec2);
	//mBody->SetLinearVelocity(vec2);
	//pixelVY = 0.f;
	//mBody->SetAngularVelocity(Utility::toRadian(Utility::vectorToDegree(sf::Vector2f(pixelVY, pixelVX), false)));
}

void Body2dComponent::setVelocity(const sf::Vector2f& pixelV, bool withSpeed)
{
	setVelocity(pixelV.x, pixelV.y, withSpeed);
}

void Body2dComponent::setVelocityScript(float pixelVX, float pixelVY, bool withSpeed)
{
	setVelocity(pixelVX, pixelVY, withSpeed);
}

void Body2dComponent::applyLinearPixelImpulseToCenter(const sf::Vector2f& impulse)
{
	mBody->ApplyLinearImpulse(Utility::formatVecPixelToMeter(impulse), mBody->GetWorldCenter(), false);
}

void Body2dComponent::applyPixelForceToCenter(const sf::Vector2f& forcePixel)
{
	mBody->ApplyForceToCenter(Utility::formatVecPixelToMeter(forcePixel), true);
}

void Body2dComponent::setMass(float32 mass)
{
	
}

//box2d uses radian
void Body2dComponent::setAngle(float32 radian)
{
	mBody->SetTransform(mBody->GetPosition(), radian);
	mFacingDir = Math::degreeToVector(Math::toDegree(radian));
}

void Body2dComponent::rotateAngle(float32 radian)
{
	float finalRadian = mBody->GetAngle() + radian;
	setAngle(finalRadian);
}

void Body2dComponent::setAngleDegree(float32 degree)
{
	setAngle(Math::toRadian(degree));
}

void Body2dComponent::rotateAngleDegree(float32 degree)
{
	rotateAngle(Math::toRadian(degree));
}


void Body2dComponent::addFixtureData(BaseFixtureData::RefID refId,
	BaseFixtureData::Ptr fixtureDataPtr, const std::string& fixtureName)
{
	auto& fixtureLogic = getFixtureLogic(fixtureName);
	fixtureLogic.mFixtureDataList.insert(std::make_pair(refId, std::move(fixtureDataPtr)));
}

void Body2dComponent::setFixtureDrawColor(const std::string& fixtureName, const sf::Color& color)
{
	getFixtureLogic(fixtureName).mDrawColor = color;
}

void Body2dComponent::setMainFixtureVerticesShapeScript(lua_State* luaState)
{
	if (mFixtureLogicList.empty())
		return;
	
	luabridge::LuaRef verticesTable = luabridge::LuaRef::fromStack(luaState, 2);
	std::size_t verticeIndex = 1;

	std::vector<sf::Vector2f> pixelVList;
	while (!verticesTable[verticeIndex].isNil()){
		luabridge::LuaRef vertTable = verticesTable[verticeIndex].cast<luabridge::LuaRef>();
		pixelVList.push_back(sf::Vector2f(vertTable[1].cast<float>(), vertTable[2].cast<float>()));
		verticeIndex++;
	}
	setFixtureVerticesShape(pixelVList, "");
}

void Body2dComponent::setFixtureVerticesShape(const std::vector<sf::Vector2f>& pixelVerticesList, const std::string& fixtureName)
{
	b2Shape* shape = fixtureName != "" ? getFixture(fixtureName)->GetShape() : getMainFixture()->GetShape();
	

	if (!shape || shape->GetType() != b2Shape::e_polygon)
		return;

	std::vector<b2Vec2> meterVList;
	meterVList.resize(pixelVerticesList.size());
	std::transform(pixelVerticesList.begin(), pixelVerticesList.end(), meterVList.begin(),
		[&](const sf::Vector2f& pixelV) -> b2Vec2
	{
		return Utility::formatVecPixelToMeter(pixelV);
	});
	setFixtureVerticesShape(meterVList, fixtureName);
}


void Body2dComponent::setFixtureVerticesShape(const std::vector<b2Vec2>& meterVerticesList, const std::string& fixtureName)
{
	b2Shape* shape = fixtureName != "" ? getFixture(fixtureName)->GetShape() : getMainFixture()->GetShape();

	if (!shape || shape->GetType() != b2Shape::e_polygon)
		return;

	b2PolygonShape* polygonShape = dynamic_cast<b2PolygonShape*>(shape);
	polygonShape->Set(&(*meterVerticesList.begin()), meterVerticesList.size());
}

void Body2dComponent::setFixtureVerticesShapeScript(const std::string& fixtureName, lua_State* luaState)
{
	if (mFixtureLogicList.empty())
		return;

	luabridge::LuaRef verticesTable = luabridge::LuaRef::fromStack(luaState, 3);
	std::size_t verticeIndex = 1;

	std::vector<sf::Vector2f> pixelVList;
	while (!verticesTable[verticeIndex].isNil()){
		luabridge::LuaRef vertTable = verticesTable[verticeIndex].cast<luabridge::LuaRef>();
		pixelVList.push_back(sf::Vector2f(vertTable[1].cast<float>(), vertTable[2].cast<float>()));
		verticeIndex++;
	}
	setFixtureVerticesShape(pixelVList, fixtureName);
}


void Body2dComponent::move(const sf::Vector2f& pixelMovement)
{
	mBody->SetTransform(mBody->GetPosition() + Utility::formatVecPixelToMeter(pixelMovement), 
		mBody->GetAngle());
}

void Body2dComponent::setBodyAwake(bool flag)
{
	mBody->SetAwake(flag);
}


b2CircleShape* Body2dComponent::getMainCircleShape() const
{
	
	b2Fixture* mainFixture = getMainFixture();
	if(mainFixture->GetType() != b2Shape::e_circle)
		return nullptr;

	return static_cast<b2CircleShape*>(mainFixture->GetShape());
}

b2PolygonShape* Body2dComponent::getMainPolyShape() const
{
	
	b2Fixture* mainFixture = getMainFixture();
	if (mainFixture->GetType() != b2Shape::e_polygon)
		return nullptr;

	return static_cast<b2PolygonShape*>(mainFixture->GetShape());
}

b2EdgeShape* Body2dComponent::getMainEdgeShape() const
{
	b2Fixture* mainFixture = getMainFixture();
	if (mainFixture->GetType() != b2Shape::e_edge)
		return nullptr;

	return static_cast<b2EdgeShape*>(mainFixture->GetShape());
}

b2Fixture* Body2dComponent::getFixture(const std::string& fixtureName) const
{
	std::string lowerFixtureName = Utility::transformStrToLowerCase(fixtureName);
	auto iter = std::find_if(mFixtureLogicList.begin(), mFixtureLogicList.end(), 
		[&](const FixtureLogic& fixtureLogic){
		return Utility::transformStrToLowerCase(fixtureLogic.mFixtureName) 
			== lowerFixtureName;
	});

	if (iter == mFixtureLogicList.end())
		return nullptr;
	return iter->mFixture;
}

void Body2dComponent::setFixtureActive(const std::string& fixtureName, bool flag)
{
	auto& curFixtureLogic = getFixtureLogic(fixtureName);
	curFixtureLogic.mIsActive = flag;

	b2Filter filterData = curFixtureLogic.mFixture->GetFilterData();

	if (curFixtureLogic.mIsActive){
		filterData.maskBits = curFixtureLogic.mOriCategoryToBeCollided;
		curFixtureLogic.mCategoryToBeCollisionSolved = 
			curFixtureLogic.mOriCategoryToBeCollisionSolved;
		curFixtureLogic.mFixture->SetFilterData(filterData);
		return;
	}
		
	filterData.maskBits = 0;
	curFixtureLogic.mFixture->SetFilterData(filterData);
	curFixtureLogic.mCategoryToBeCollisionSolved = 0;
}

const FixtureLogic& Body2dComponent::getFixtureLogic(const std::string& fixtureName) const
{
	std::string lowerFixtureName = Utility::transformStrToLowerCase(fixtureName);
	auto iter = std::find_if(mFixtureLogicList.begin(), mFixtureLogicList.end(),
		[&](const FixtureLogic& fixtureLogic){
		return Utility::transformStrToLowerCase(fixtureLogic.mFixtureName)
			== lowerFixtureName;
	});
	assert(iter != mFixtureLogicList.end());
	return *iter;
}

const FixtureLogic& Body2dComponent::getConstFixtureLogic(const std::string& fixtureName) const
{
	return getFixtureLogic(fixtureName);
}

FixtureLogic& Body2dComponent::getFixtureLogic(const std::string& fixtureName)
{
	std::string lowerFixtureName = Utility::transformStrToLowerCase(fixtureName);
	auto iter = std::find_if(mFixtureLogicList.begin(), mFixtureLogicList.end(),
		[&](const FixtureLogic& fixtureLogic){
		return Utility::transformStrToLowerCase(fixtureLogic.mFixtureName)
			== lowerFixtureName;
	});
	assert(iter != mFixtureLogicList.end());
	return *iter;
}



const FixtureLogic& Body2dComponent::getMainFixtureLogic() const
{
	return mFixtureLogicList[mIndexOfMainFixture];
}


b2Fixture* Body2dComponent::getMainFixture() const
{
	if (mFixtureLogicList.empty())
		return nullptr;
	return getMainFixtureLogic().mFixture;
}


void Body2dComponent::setMainFixtureNewShape(b2Shape* newShape)
{
	setNewShape(newShape, mFixtureLogicList[mIndexOfMainFixture]);
	/*b2Fixture* mainFixture = getMainFixture();
	b2FixtureDef fixtureDef;
	fixtureDef.density = mainFixture->GetDensity();
	fixtureDef.filter = mainFixture->GetFilterData();
	fixtureDef.friction = mainFixture->GetFriction();
	fixtureDef.restitution = mainFixture->GetRestitution();
	fixtureDef.userData = mainFixture->GetUserData();
	fixtureDef.shape = newShape.release();

	mBody->DestroyFixture(mainFixture);
	auto& fixtureLogic = mFixtureLogicList[mIndexOfMainFixture];
	fixtureLogic.mFixture = mBody->CreateFixture(&fixtureDef);*/
	//delete fixtureDef.shape;
}

void Body2dComponent::setNewShape(b2Shape* newShape, FixtureLogic& fixtureLogic)
{
	auto fixture = fixtureLogic.mFixture;

	b2FixtureDef fixtureDef;
	fixtureDef.density = fixture->GetDensity();
	fixtureDef.filter = fixture->GetFilterData();
	fixtureDef.friction = fixture->GetFriction();
	fixtureDef.restitution = fixture->GetRestitution();
	fixtureDef.userData = fixture->GetUserData();
	fixtureDef.shape = newShape;

	mBody->DestroyFixture(fixture);
	fixtureLogic.mFixture = mBody->CreateFixture(&fixtureDef);
}

void Body2dComponent::setFixtureNewShape(b2Shape* newShape, const std::string& fixtureName)
{
	setNewShape(newShape, getFixtureLogic(fixtureName));
}

b2Shape* Body2dComponent::cloneMainFixtureShape(b2BlockAllocator* blockAllocator) const
{
	b2Fixture* mainFixture = getMainFixture();
	return mainFixture->GetShape()->Clone(blockAllocator);
}

b2Shape* Body2dComponent::cloneFixtureShape(const std::string& fixtureName, b2BlockAllocator* blockAllocator) const
{
	auto& fixtureLogic =  getConstFixtureLogic(fixtureName);
	return fixtureLogic.mFixture->GetShape()->Clone(blockAllocator);
}

void Body2dComponent::setMainFixtureActive(bool flag)
{
	assert(!mFixtureLogicList.empty());
	setFixtureActive(mMainFixtureName, flag);
	//mFixtureLogicList[mIndexOfMainFixture].mIsActive = flag;
}

bool Body2dComponent::isMainFixtureActive(bool flag) const
{
	assert(!mFixtureLogicList.empty());
	return mFixtureLogicList[mIndexOfMainFixture].mIsActive;
}

void Body2dComponent::setGravityScale(float val)
{
	mBody->SetGravityScale(val);
}


sf::Vector2f Body2dComponent::getFacingDir() const
{
	return mFacingDir;
}

float Body2dComponent::getFacingDirX() const
{
	return getFacingDir().x;
}

float Body2dComponent::getFacingDirY() const
{
	return getFacingDir().y;
}

float Body2dComponent::get_steering_turning_rate() const
{
	return m_steering_turning_rate;
}
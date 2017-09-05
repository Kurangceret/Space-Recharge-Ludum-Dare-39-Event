#include "JumpComponent.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"

JumpComponent::JumpComponent(Entity* entity)
:Component(entity, ComponentIdentifier::JumpComponent),
mMaxNumberOfJumpLimit(1),
mCurNumberOfJumpLimit(1),
mCurrentJumpCount(0),
mWillProlongJump(false),
/*mIncJumpPosPerSec(0.f),
mMaxIncJumpPos(0.f),
mCurIncJumpPos(0.f),*/
mJumpVeloHoldVal(0.f),
mMaxTimeJumpVeloHold(sf::Time::Zero),
mCurTimeJumpVeloHold(sf::Time::Zero),
mJumpImpulse(0.f)
{
}


JumpComponent::~JumpComponent()
{
}

void JumpComponent::startJumpImpulse(float jumpImpulse)
{
	if (mCurrentJumpCount >= mCurNumberOfJumpLimit)
		return;

	auto body2dComp = getOwnerEntity()->nonCreateComp<Body2dComponent>();

	if (jumpImpulse == 0.f)
		jumpImpulse = mJumpImpulse;

	body2dComp->setVelocityY(0.f, false);
	body2dComp->applyLinearPixelImpulseToCenter(sf::Vector2f(0.f, (std::abs(jumpImpulse) * -1.f)));
	mCurrentJumpCount++;

	mCurTimeJumpVeloHold = mMaxTimeJumpVeloHold;
	//mCurIncJumpPos = mMaxIncJumpPos;
	//mWillProlongJump = true;
}

bool JumpComponent::isJumping() const
{
	return mCurrentJumpCount > 0;
}

void JumpComponent::resetJumpCount()
{
	mCurrentJumpCount = 0;
	mWillProlongJump = false;
	mCurTimeJumpVeloHold = sf::Time::Zero;
	//mCurIncJumpPos = 0.f;
}

void JumpComponent::prolongJump()
{
	mWillProlongJump = true;
}

bool JumpComponent::isOnGround() const
{
	return !mGroundEntities.empty();
}

void JumpComponent::detachGroundEntity(const Entity* entity)
{
	auto iter = getGroundIter(entity);
	if (iter == mGroundEntities.end())
		return;
	mGroundEntities.erase(iter);
}

void JumpComponent::attachGroundEntity(const Entity* entity)
{
	if (getGroundIter(entity) != mGroundEntities.end())
		return;

	mGroundEntities.push_back(entity);
}

JumpComponent::GroundEntities::const_iterator 
	JumpComponent::getGroundIter(const Entity* entity) const
{
	return std::find(mGroundEntities.begin(), mGroundEntities.end(), entity);
}

int JumpComponent::getMaxNumOfJumpLimit() const
{
	return mMaxNumberOfJumpLimit;
}

void JumpComponent::setCurNumberOfJumpLimit(int newVal)
{
	if (newVal < 0) newVal = 0;
	if (newVal > mMaxNumberOfJumpLimit) newVal = mMaxNumberOfJumpLimit;

	mCurNumberOfJumpLimit = newVal;
}
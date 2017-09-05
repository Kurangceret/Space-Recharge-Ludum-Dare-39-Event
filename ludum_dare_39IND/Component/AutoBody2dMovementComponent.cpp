#include "AutoBody2dMovementComponent.h"
#include "ComponentIdentifier.h"



AutoMovementSetData::AutoMovementSetData()
:mUseStartingPos(false)
{

}
AutoMovementSetData::AutoMovementSetData(AutoMovementSetData& autoMovementSetData)
:mUseStartingPos(autoMovementSetData.mUseStartingPos),
mIsStartingPosAbsolute(autoMovementSetData.mIsStartingPosAbsolute),
mStartOffsetPos(autoMovementSetData.mStartOffsetPos),
mStartOffsetDegree(autoMovementSetData.mStartOffsetDegree),
mUseStartingDegree(autoMovementSetData.mUseStartingDegree),
mSingleMoveDataList(autoMovementSetData.mSingleMoveDataList),
mParentMoveDataList(autoMovementSetData.mParentMoveDataList),
mBaseShapePtr(std::move(autoMovementSetData.mBaseShapePtr)),
mOnDelayMoveFunc(std::move(autoMovementSetData.mOnDelayMoveFunc)),
mOnActiveMoveFunc(std::move(autoMovementSetData.mOnActiveMoveFunc))
{

}


AutoBody2dMovementComponent::CustomData::CustomData()
:mUseCustomData(false),
mReverseDataForPositiveBaseMovDir(false)
{

}

AutoBody2dMovementComponent::AutoBody2dMovementComponent(Entity* entity)
:Component(entity, ComponentIdentifier::AutoBody2dMovementComponent)
{
}


AutoBody2dMovementComponent::~AutoBody2dMovementComponent()
{
}


void AutoBody2dMovementComponent::startMovement(const sf::Vector2f& autoMovDir,
	const std::string& autoMovementName, const CustomData& customData)
{
	mCurMovementName = autoMovementName;
	mCurAutoMovBaseDir = autoMovDir;
	mIsRequestingSystem = true;
	mCustomData = customData;
}

void AutoBody2dMovementComponent::stopCurMovement()
{
	mRequestSystemToStop = true;
}

bool AutoBody2dMovementComponent::isMovingNow() const
{
	return mIsMoving;
}

std::string AutoBody2dMovementComponent::getCurrentMovementName() const
{
	return mCurMovementName;
}

const AutoMovementSetData& AutoBody2dMovementComponent::getConstAutoMovementSetData(
	const std::string& autoMovementName) const
{
	auto findIter = getAutoMovementIter(autoMovementName);
	assert(findIter != mAutoMovementList.end());
	return findIter->second;
}

AutoMovementSetData& AutoBody2dMovementComponent::getAutoMovementSetData(
	const std::string& autoMovementName)
{
	auto findIter = getAutoMovementIter(autoMovementName);
	assert(findIter != mAutoMovementList.end());
	return findIter->second;
}

AutoBody2dMovementComponent::AutoMovementMap::const_iterator AutoBody2dMovementComponent::
	getAutoMovementIter(const std::string& autoMovementName) const
{
	return mAutoMovementList.find(autoMovementName);
}

AutoBody2dMovementComponent::AutoMovementMap::iterator AutoBody2dMovementComponent::
	getAutoMovementIter(const std::string& autoMovementName)
{
		return mAutoMovementList.find(autoMovementName);
}

sf::Vector2f AutoBody2dMovementComponent::getCurAutoMovBaseDir() const
{
	return mCurAutoMovBaseDir;
}
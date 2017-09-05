#include "RealTimeInputComponent.h"
#include "ComponentIdentifier.h"

RealTimeInputComponent::RealTimeInputComponent(Entity* entity)
:Component(entity, ComponentIdentifier::RealTimeInputComponent),
mCurDoubleTappedKey(sf::Keyboard::Unknown)
{
	/*std::size_t mJumpKeyPressedCount;
	std::size_t mDashKeyPressedCount;
	std::size_t mAttackKeyPressedCount;

	std::size_t mUpKeyPressedCount;
	std::size_t mDownKeyPressedCount;
	std::size_t mLeftKeyPressedCount;
	std::size_t mRightKeyPressedCount;*/
	mEventKeyList.push_back(&mJumpKeyPressedCount);
	mEventKeyList.push_back(&mDashKeyPressedCount);
	mEventKeyList.push_back(&mAttackKeyPressedCount);

	mEventKeyList.push_back(&mUpKeyPressedCount);
	mEventKeyList.push_back(&mDownKeyPressedCount);
	mEventKeyList.push_back(&mLeftKeyPressedCount);
	mEventKeyList.push_back(&mRightKeyPressedCount);
	
}


RealTimeInputComponent::~RealTimeInputComponent()
{
}


void RealTimeInputComponent::emptyAllEventKey(std::size_t* exceptKey)
{
	for (auto& keyPointer : mEventKeyList){
		if (keyPointer == exceptKey)
			continue;
		*keyPointer = 0;
	}
}

void RealTimeInputComponent::emptyDashKeyPressed()
{
	mDashKeyPressedCount = 0;
}

void RealTimeInputComponent::emptyJumpKeyPressed()
{
	mJumpKeyPressedCount = 0;
}

void RealTimeInputComponent::emptyAttackKeyPressed()
{
	mAttackKeyPressedCount = 0;
}

bool RealTimeInputComponent::isJumpKeyPressed() const
{
	return mJumpKeyPressedCount > 0;
}

bool RealTimeInputComponent::isDashKeyPressed() const
{
	return mDashKeyPressedCount > 0;
}

bool RealTimeInputComponent::isAttackKeyPressed() const
{
	return mAttackKeyPressedCount > 0;
}

bool RealTimeInputComponent::isUpKeyPressed() const
{
	return mUpKeyPressedCount > 0;
}

bool RealTimeInputComponent::isDownKeyPressed() const
{
	return mDownKeyPressedCount > 0;
}

bool RealTimeInputComponent::isRightKeyPressed() const
{
	return mRightKeyPressedCount > 0;
}

bool RealTimeInputComponent::isLeftKeyPressed() const
{
	return mLeftKeyPressedCount > 0;
}



bool RealTimeInputComponent::isUpKeyCountEmpty() const
{
	return mUpKeyCount <= 0;
}

bool RealTimeInputComponent::isDownKeyCountEmpty() const
{
	return mDownKeyCount <= 0;
}

bool RealTimeInputComponent::isLeftKeyCountEmpty() const
{
	return mLeftKeyCount <= 0;
}

bool RealTimeInputComponent::isRightKeyCountEmpty() const
{
	return mRightKeyCount <= 0;
}

sf::Keyboard::Key RealTimeInputComponent::getCurDoubleTappedKey() const
{
	return mCurDoubleTappedKey;
}
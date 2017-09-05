#include "RealTimeInputSystem.h"
#include "../Entity/Entity.h"
#include "../Component/RealTimeInputComponent.h"
#include "../Component/PlayerComponent.h"
#include <SFML/System/Vector2.hpp>


RealTimeInputSystem::RealTimeInputSystem()
:mMaxInputCount(3),
mCurKey(sf::Keyboard::Unknown),
mPlayerInputComp(nullptr),
mMaxWaitTimeDoubleTap(sf::seconds(0.5)),
mCurWaitTimeDoubleTap(sf::Time::Zero),
mMaxPressedCount(4)
{
	pushRequiredComponent(ComponentIdentifier::RealTimeInputComponent);
}


RealTimeInputSystem::~RealTimeInputSystem()
{
}

void RealTimeInputSystem::handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event)
{
	if (mPlayerInputComp && event.type == sf::Event::KeyPressed)
	{
		
		bool dashKeyPressed = event.key.code == sf::Keyboard::Z;
		bool jumpKeyPressed = event.key.code == sf::Keyboard::X;
		bool attackKeyPressed = event.key.code == sf::Keyboard::C;

		bool upKeyPressed = event.key.code == sf::Keyboard::Up;
		bool downKeyPressed = event.key.code == sf::Keyboard::Down;
		bool leftKeyPressed = event.key.code == sf::Keyboard::Left;
		bool rightKeyPressed = event.key.code == sf::Keyboard::Right;

		if (mPlayerInputComp->mDashKeyPressedCount < mMaxPressedCount && dashKeyPressed){
			mPlayerInputComp->mDashKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mDashKeyPressedCount);
		}
		if (mPlayerInputComp->mJumpKeyPressedCount < mMaxPressedCount && jumpKeyPressed){
			mPlayerInputComp->mJumpKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mJumpKeyPressedCount);
		}
		if (mPlayerInputComp->mAttackKeyPressedCount < mMaxPressedCount && attackKeyPressed){
			mPlayerInputComp->mAttackKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mAttackKeyPressedCount);
		}



		if (mPlayerInputComp->mUpKeyPressedCount < mMaxPressedCount && upKeyPressed){
			mPlayerInputComp->mUpKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mUpKeyPressedCount);
		}
		if (mPlayerInputComp->mDownKeyPressedCount < mMaxPressedCount && downKeyPressed){
			mPlayerInputComp->mDownKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mDownKeyPressedCount);
		}
		if (mPlayerInputComp->mLeftKeyPressedCount < mMaxPressedCount && leftKeyPressed){
			mPlayerInputComp->mLeftKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mLeftKeyPressedCount);
		}
		if (mPlayerInputComp->mRightKeyPressedCount < mMaxPressedCount && rightKeyPressed){
			mPlayerInputComp->mRightKeyPressedCount = mMaxPressedCount;
			mPlayerInputComp->emptyAllEventKey(&mPlayerInputComp->mRightKeyPressedCount);
		}
		
			
	}
	

	if (mNextFrameSetAllDoubleTap)
		return;

	//if (!mPlayerComp)
		//return;

	//sf::Vector2f prevGravDir = mPlayerComp->getPrevGravityDir();

	if (event.type == sf::Event::KeyPressed){
		auto nowKey = event.key.code;
		if (nowKey == mCurKey){
			mNextFrameSetAllDoubleTap = true;
			return;
		}
		mCurKey = nowKey;
		mCurWaitTimeDoubleTap = mMaxWaitTimeDoubleTap;
	}

}

bool RealTimeInputSystem::initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	if (mCurWaitTimeDoubleTap <= sf::Time::Zero || mNextFrameSetAllDoubleTap)
		return true;

	mCurWaitTimeDoubleTap -= dt;

	if (mCurWaitTimeDoubleTap <= sf::Time::Zero)
		mCurKey = sf::Keyboard::Unknown;
	return true;
}


void RealTimeInputSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	RealTimeInputComponent* realTimeInputComp = entity->getComp<RealTimeInputComponent>();
	if (!mPlayerInputComp)
		mPlayerInputComp = realTimeInputComp;

	if (realTimeInputComp->mCurDoubleTappedKey != sf::Keyboard::Unknown)
		realTimeInputComp->mCurDoubleTappedKey = sf::Keyboard::Unknown;

	if (mNextFrameSetAllDoubleTap){
		realTimeInputComp->mCurDoubleTappedKey = mCurKey;
		mNextFrameSetAllDoubleTap = false;
		mCurWaitTimeDoubleTap = sf::Time::Zero;
		mCurKey = sf::Keyboard::Unknown;
		std::cout << "double tapped" << std::endl;
	}


	if (realTimeInputComp->mJumpKeyPressedCount > 0)
		realTimeInputComp->mJumpKeyPressedCount--;
	if (realTimeInputComp->mDashKeyPressedCount > 0)
		realTimeInputComp->mDashKeyPressedCount--;
	if (realTimeInputComp->mAttackKeyPressedCount > 0)
		realTimeInputComp->mAttackKeyPressedCount--;

	if (realTimeInputComp->mUpKeyPressedCount > 0)
		realTimeInputComp->mUpKeyPressedCount--;
	if (realTimeInputComp->mDownKeyPressedCount > 0)
		realTimeInputComp->mDownKeyPressedCount--;
	if (realTimeInputComp->mLeftKeyPressedCount > 0)
		realTimeInputComp->mLeftKeyPressedCount--;
	if (realTimeInputComp->mRightKeyPressedCount > 0)
		realTimeInputComp->mRightKeyPressedCount--;

	bool isUpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool isDownKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool isLeftKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool isRightKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

	if (isUpKeyPressed && isLeftKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		std::cout << "lul" << std::endl;

	if (realTimeInputComp->mUpKeyCount < mMaxInputCount && isUpKeyPressed){
		realTimeInputComp->mUpKeyCount++;
		realTimeInputComp->mDownKeyCount = 0;
	}
	else if (!isUpKeyPressed && realTimeInputComp->mUpKeyCount > 0) 
		realTimeInputComp->mUpKeyCount--;

	if (realTimeInputComp->mDownKeyCount < mMaxInputCount && isDownKeyPressed){
		realTimeInputComp->mDownKeyCount++;
		realTimeInputComp->mUpKeyCount = 0;
	}
	else if (!isDownKeyPressed && realTimeInputComp->mDownKeyCount > 0)
		realTimeInputComp->mDownKeyCount--;

	if (realTimeInputComp->mLeftKeyCount< mMaxInputCount && isLeftKeyPressed){
		realTimeInputComp->mLeftKeyCount++;
		realTimeInputComp->mRightKeyCount = 0;
	}
	else if (!isLeftKeyPressed && realTimeInputComp->mLeftKeyCount > 0)
		realTimeInputComp->mLeftKeyCount--;

	if (realTimeInputComp->mRightKeyCount < mMaxInputCount && isRightKeyPressed){
		realTimeInputComp->mRightKeyCount++;
		realTimeInputComp->mLeftKeyCount = 0;
	}
	else if (!isRightKeyPressed && realTimeInputComp->mRightKeyCount > 0)
		realTimeInputComp->mRightKeyCount--;
}
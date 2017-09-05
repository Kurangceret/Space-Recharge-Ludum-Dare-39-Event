#include "AutoBody2dMovementSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Helper/Utility.h"
#include "../Helper/Constant.h"
#include "../Helper/Math.h"
#include "../Component/EntityChildrenComponent.h"
#include <Box2D\Collision\Shapes\b2PolygonShape.h>

AutoBody2dMovementSystem::AutoBody2dMovementSystem()
{
	pushRequiredComponent(ComponentIdentifier::AutoBody2dMovementComponent);
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);
}


AutoBody2dMovementSystem::~AutoBody2dMovementSystem()
{
}


ProcessAutoSingleMovData::ProcessAutoSingleMovData()
{}
ProcessAutoSingleMovData::ProcessAutoSingleMovData(ProcessAutoSingleMovData& right)
:mElapTimeSingleMov(right.mElapTimeSingleMov),
mCurSingleMovIndex(right.mCurSingleMovIndex),
mMovDataList(right.mMovDataList),
mOriginalTransform(right.mOriginalTransform),
mOriginalShapePtr(right.mOriginalShapePtr.release()),
mIsProcessDone(right.mIsProcessDone)
{

}

ProcessAutoSingleMovData& ProcessAutoSingleMovData::operator = (ProcessAutoSingleMovData& right)
{
	mElapTimeSingleMov = right.mElapTimeSingleMov;
	mCurSingleMovIndex = right.mCurSingleMovIndex;
	mMovDataList = right.mMovDataList;
	mOriginalTransform = right.mOriginalTransform;
	mOriginalShapePtr = std::move(right.mOriginalShapePtr);
	mIsProcessDone = right.mIsProcessDone;
	return *this;
}

void AutoBody2dMovementSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	AutoBody2dMovementComponent* autoBody2dMovComp = entity->getComp<AutoBody2dMovementComponent>();
	Body2dComponent* body2dComp = entity->getComp<Body2dComponent>();

	auto iter = getProcessAutoBody2dMapIter(autoBody2dMovComp);

	if (autoBody2dMovComp->mIsRequestingSystem){
		if (iter != mProcessAutoBody2dMap.end()){
			auto& procSingleMovData = iter->second.mProcAutoSingleMovData;
			auto& procParentMovData = iter->second.mProcAutoParentMovData;

			finalizeToBeDeletedProcess(autoBody2dMovComp, body2dComp,
				entity->nonCreateComp<EntityChildrenComponent>(),
				entity, entityManager, procSingleMovData, procParentMovData);
		}
		processRequestForEntry(entity, autoBody2dMovComp,  body2dComp);
		return;
	}
	EntityChildrenComponent* entityChildComp = entity->nonCreateComp<EntityChildrenComponent>();

	if (iter == mProcessAutoBody2dMap.end())
		return;

	
	auto& procSingleMovData = iter->second.mProcAutoSingleMovData;
	auto& procParentMovData = iter->second.mProcAutoParentMovData;
	

	const bool singleProcessDone = procSingleMovData.mIsProcessDone;
	const bool parentProcessDone = !entityChildComp || procParentMovData.mIsProcessDone;

	if (autoBody2dMovComp->mRequestSystemToStop || (singleProcessDone && parentProcessDone))
	{
		finalizeToBeDeletedProcess(autoBody2dMovComp, body2dComp, entityChildComp,
			entity, entityManager, procSingleMovData, procParentMovData);
		return;
	}

	const AutoMovementSetData& autoMovSetData = autoBody2dMovComp->
		getAutoMovementSetData(autoBody2dMovComp->mCurMovementName);

	if (!singleProcessDone)
		resumeProcessAutoSingleMovementData(autoBody2dMovComp, body2dComp,
			procSingleMovData, autoMovSetData, entityManager, dt);

	if (!parentProcessDone){
		resumeProcessAutoParentMovementData(autoBody2dMovComp, body2dComp, 
			entityChildComp, procParentMovData, autoMovSetData, entityManager, dt);
	}
	
}

void AutoBody2dMovementSystem::resumeProcessAutoSingleMovementData(AutoBody2dMovementComponent* autoBody2dMovComp,
	Body2dComponent* body2dComp, ProcessAutoSingleMovData& procAutoSingleMovData, 
	const AutoMovementSetData& autoMovSetData, EntityManager* entityManager, sf::Time dt)
{
	sf::Time& elapTimeSingleMov = procAutoSingleMovData.mElapTimeSingleMov;
	elapTimeSingleMov += dt;
	const auto* movDataList = &procAutoSingleMovData.mMovDataList[procAutoSingleMovData.mCurSingleMovIndex];

	sf::Time totalTime = movDataList->mActiveDur + movDataList->mDelayDur;
	if (elapTimeSingleMov > totalTime){
		procAutoSingleMovData.mCurSingleMovIndex++;
		elapTimeSingleMov -= totalTime;
		if (procAutoSingleMovData.mCurSingleMovIndex >= procAutoSingleMovData.mMovDataList.size()){
			procAutoSingleMovData.mIsProcessDone = true;
			body2dComp->setVelocity(0.f, 0.f, true);
			body2dComp->setMainFixtureActive(false);
			return;
		}
		movDataList = &procAutoSingleMovData.mMovDataList[procAutoSingleMovData.mCurSingleMovIndex];
	}
	body2dComp->setMainFixtureActive(false);

	if (procAutoSingleMovData.mOnDelayFunc){
		(*procAutoSingleMovData.mOnDelayFunc)(autoBody2dMovComp->getOwnerEntity(), entityManager);
		procAutoSingleMovData.mOnDelayFunc = nullptr;
	}

	if (elapTimeSingleMov > movDataList->mDelayDur){
		body2dComp->setMainFixtureActive(true);
		//body2dComp->setBaseSpeed(movDataList->mSpeedPerSec);
		body2dComp->setVelocity(movDataList->mMovDir * movDataList->mSpeedPerSec, false);
		body2dComp->setAngleDegree(body2dComp->getAngleDegree() + 
			(movDataList->mDegreeIdentPerSec * dt.asSeconds()));

		if (procAutoSingleMovData.mOnActiveFunc){
			(*procAutoSingleMovData.mOnActiveFunc)(autoBody2dMovComp->getOwnerEntity(), entityManager);
			procAutoSingleMovData.mOnActiveFunc = nullptr;
		}

		return;
	}
	body2dComp->setVelocity(0.f, 0.f, false);
}

void AutoBody2dMovementSystem::resumeProcessAutoParentMovementData(AutoBody2dMovementComponent* autoBody2dMovComp,
	Body2dComponent* body2dComp,
	EntityChildrenComponent* entityChildComp,
	ProcessAutoParentMovData& procAutoParentMovData, 
	const AutoMovementSetData& autoMovSetData, EntityManager* entityManager, sf::Time dt)
{
	sf::Time& elapTimeParentMov = procAutoParentMovData.mElapTimeParentMov;
	elapTimeParentMov += dt;
	const auto* movDataList = &procAutoParentMovData.mMovDataList[procAutoParentMovData.mCurParentMovIndex];


	Entity* parentEntity = entityChildComp->getParent();
	Body2dComponent* parentBody2dComp = parentEntity->getComp<Body2dComponent>();
	

	sf::Time totalTime = movDataList->mActiveDur + movDataList->mDelayDur;
	if (elapTimeParentMov > totalTime){
		procAutoParentMovData.mCurParentMovIndex++;
		elapTimeParentMov -= totalTime;
		if (procAutoParentMovData.mCurParentMovIndex >= procAutoParentMovData.mMovDataList.size()){
			procAutoParentMovData.mIsProcessDone = true;
			parentBody2dComp->setVelocity(0.f, 0.f, false);
			return;
		}
		movDataList = &procAutoParentMovData.mMovDataList[procAutoParentMovData.mCurParentMovIndex];
	}
	
	if (elapTimeParentMov > movDataList->mDelayDur){
		parentBody2dComp->setVelocity(movDataList->mMovDir * movDataList->mSpeedPerSec, false);
		//parentComp->setCurSpeed(movDataList->mSpeedPerSec);
		//parentComp->setVelocity(movDataList->mMovDir, false);
		if (movDataList->mDegreeIdentPerSec != 0.f)
			parentBody2dComp->setAngleDegree(parentBody2dComp->getAngleDegree() 
				+ (movDataList->mDegreeIdentPerSec * dt.asSeconds()));
		return;
	}
	parentBody2dComp->setVelocity(0.f, 0.f, false);
}


void AutoBody2dMovementSystem::processRequestForEntry(Entity* entity, 
	AutoBody2dMovementComponent* autoBody2dMovComp, 
	Body2dComponent* body2dComp)
{
	autoBody2dMovComp->mIsRequestingSystem = false;

	auto findIter = getProcessAutoBody2dMapIter(autoBody2dMovComp);
	if (findIter == mProcessAutoBody2dMap.end())
		mProcessAutoBody2dMap.insert(std::make_pair(autoBody2dMovComp, ProcessAutoMovementData()));

	auto& processAutoMovData = mProcessAutoBody2dMap[autoBody2dMovComp];
	auto& procAutoSingleMovData = processAutoMovData.mProcAutoSingleMovData;
	auto& procAutoParentMovData = processAutoMovData.mProcAutoParentMovData;
	processAutoMovData.mAutoMovSetName = autoBody2dMovComp->mCurMovementName;

	auto& autoMovementSetData = autoBody2dMovComp->getAutoMovementSetData(processAutoMovData.mAutoMovSetName);

	processInitialSingleMovEntry(autoBody2dMovComp,  body2dComp, procAutoSingleMovData, autoMovementSetData);
	EntityChildrenComponent* entityChildComp = entity->nonCreateComp<EntityChildrenComponent>();

	if (entityChildComp)
		processInitialParentMovEntry(autoBody2dMovComp,  body2dComp, entityChildComp, procAutoParentMovData, autoMovementSetData);

	autoBody2dMovComp->mIsMoving = true;
}

void AutoBody2dMovementSystem::processInitialSingleMovEntry(AutoBody2dMovementComponent* autoBody2dMovComp,
	Body2dComponent* body2dComp,
	ProcessAutoSingleMovData& procAutoSingleMovData,
	const AutoMovementSetData& autoMovementSetData)
{
	procAutoSingleMovData.mCurSingleMovIndex = 0;
	procAutoSingleMovData.mElapTimeSingleMov = sf::Time::Zero;
	procAutoSingleMovData.mOriginalTransform = body2dComp->getTransform(false);
	procAutoSingleMovData.mIsProcessDone = false;
	procAutoSingleMovData.mOnActiveFunc = autoMovementSetData.mOnActiveMoveFunc.get();
	procAutoSingleMovData.mOnDelayFunc = autoMovementSetData.mOnDelayMoveFunc.get();
	procAutoSingleMovData.mOnDoneFunc = autoMovementSetData.mOnDoneMoveFunc.get();

	


	sf::Vector2f baseMovDir = autoBody2dMovComp->mCurAutoMovBaseDir;


	bool reverse = (autoMovementSetData.mReverseDataForNegativeXBaseMovDir) && ((baseMovDir.x < 0.f) || (baseMovDir.y < 0.f));

	if (autoBody2dMovComp->mCustomData.mUseCustomData){
		const auto& customData = autoBody2dMovComp->mCustomData;
		reverse = customData.mReverseDataForPositiveBaseMovDir && 
			((baseMovDir.x > 0.f) || (baseMovDir.y > 0.f));
	}

	float baseMovDegree = Math::vectorToDegree(baseMovDir, false);
	if (autoMovementSetData.mUseStartingDegree){
		//float curDegree = Utility::vectorToDegree(body2dComp->getFacingDirection());
		float startOffsetDegree = autoMovementSetData.mStartOffsetDegree;
		startOffsetDegree = reverse ? startOffsetDegree * -1.f : startOffsetDegree;

		body2dComp->setAngleDegree(startOffsetDegree + baseMovDegree);
	}
	if (autoMovementSetData.mUseRelativeToBaseMovDir){
		sf::Vector2f relativeDir(Math::degreeToVector(baseMovDegree +
			autoMovementSetData.mDegreeRelToBaseMovDir));
		sf::Vector2f finalRelativePos(relativeDir.x * autoMovementSetData.mPosRelToBaseMovDir.x, 
			relativeDir.y * autoMovementSetData.mPosRelToBaseMovDir.y);

		body2dComp->setPosition(body2dComp->getSfPosition(false) + finalRelativePos);
	}
	if (autoMovementSetData.mUseStartingPos){
		sf::Vector2f finalOffsetPos;
		if (!autoMovementSetData.mIsStartingPosAbsolute){
			finalOffsetPos = sf::Vector2f(baseMovDir.x * autoMovementSetData.mStartOffsetPos.x, 
				baseMovDir.y * autoMovementSetData.mStartOffsetPos.y);
		}
		else{
			finalOffsetPos = autoMovementSetData.mStartOffsetPos;
		}

		body2dComp->setPosition(body2dComp->getSfPosition(false) + finalOffsetPos);
	}

	if (autoMovementSetData.mUseLatestDegreeForOffset){
		sf::Vector2f curBodyDir = Math::degreeToVector(body2dComp->getAngleDegree());
		sf::Vector2f offSet(curBodyDir.x * autoMovementSetData.mLatestDegreeOffsetPos.x,
			curBodyDir.y * autoMovementSetData.mLatestDegreeOffsetPos.y);

		body2dComp->setPosition(body2dComp->getSfPosition(false) + offSet);
	}
	

	//float curDegree = Utility::vectorToDegree(body2dComp->getFacingDirection());
	sf::Vector2f curPos = body2dComp->getSfPosition(false);
	
	for (auto& singleMovEntry : autoMovementSetData.mSingleMoveDataList){
		MovData movData;
		movData.mDegreeIdentPerSec = 0.f;
		movData.mSpeedPerSec = 0.f;
		movData.mActiveDur = singleMovEntry.mActiveDur;
		movData.mDelayDur = singleMovEntry.mDelayDur;
			
		const float offsetDegree = reverse ? singleMovEntry.mOffsetDegree * -1.f 
			: singleMovEntry.mOffsetDegree;

		const sf::Vector2f& offsetPos = singleMovEntry.mOffsetPos;
		float activeDurSec = singleMovEntry.mActiveDur.asSeconds();

		if (offsetDegree != 0.f)
			movData.mDegreeIdentPerSec = offsetDegree / activeDurSec;
		
		
		if (offsetPos != sf::Vector2f()){
			sf::Vector2f finalMovOffset(offsetPos.x * baseMovDir.x, offsetPos.y * baseMovDir.y);
			sf::Vector2f targetPos(curPos + finalMovOffset);

			movData.mSpeedPerSec = Math::vectorLength(targetPos - curPos) / activeDurSec;
			movData.mMovDir = Math::unitVector(targetPos - curPos);
			curPos = targetPos;
		}
		procAutoSingleMovData.mMovDataList.push_back(movData);
	}
	if (autoMovementSetData.mBaseShapePtr.get()){
		procAutoSingleMovData.mOriginalShapePtr.reset(body2dComp->cloneMainFixtureShape(&mBlockAllocator));
		//std::unique_ptr<b2Shape> shape;
		//shape.reset(autoMovementSetData.mBaseShapePtr->Clone(&mBlockAllocator));
		body2dComp->setMainFixtureNewShape(autoMovementSetData.mBaseShapePtr->Clone(&mBlockAllocator));
	}
}

void AutoBody2dMovementSystem::processInitialParentMovEntry(AutoBody2dMovementComponent* autoBody2dMovComp,
	Body2dComponent* body2dComp,
	EntityChildrenComponent* entityChildComp,
	ProcessAutoParentMovData& procAutoParentMovData,
	const AutoMovementSetData& autoMovementSetData)
{
	
	procAutoParentMovData.mCurParentMovIndex = 0;
	procAutoParentMovData.mElapTimeParentMov = sf::Time::Zero;
	procAutoParentMovData.mIsProcessDone = false;

	Entity* parentEntity = entityChildComp->getParent();
	Body2dComponent* parentBody2dComp = parentEntity->getComp<Body2dComponent>();
	

	procAutoParentMovData.mOriMovSpeed = parentBody2dComp->getBaseSpeed();
	//procAutoParentMovData.mOriSteeringBehaviorFlag = parentbody2dComp->isUsingSteeringBehavior();

	//parentbody2dComp->setSteeringBehavior(false);

	if (autoMovementSetData.mParentMoveDataList.empty()){
		procAutoParentMovData.mIsProcessDone = true;
		return;
	}

	sf::Vector2f curPos = parentBody2dComp->getSfPosition(false);
	sf::Vector2f baseMovDir = autoBody2dMovComp->mCurAutoMovBaseDir;

	for (auto& parentMovEntry : autoMovementSetData.mParentMoveDataList){
		MovData movData;
		movData.mDegreeIdentPerSec = 0.f;
		movData.mSpeedPerSec = 0.f;
		movData.mActiveDur = parentMovEntry.mActiveDur;
		movData.mDelayDur = parentMovEntry.mDelayDur;

		const float offsetDegree = parentMovEntry.mOffsetDegree;
		const sf::Vector2f& offsetPos = parentMovEntry.mOffsetPos;
		float activeDurSec = parentMovEntry.mActiveDur.asSeconds();

		if (offsetDegree != 0.f)
			movData.mDegreeIdentPerSec = offsetDegree / activeDurSec;


		if (offsetPos != sf::Vector2f()){
			sf::Vector2f finalMovOffset(offsetPos.x * baseMovDir.x, offsetPos.y * baseMovDir.y);
			sf::Vector2f targetPos(curPos + finalMovOffset);

			movData.mSpeedPerSec = Math::vectorLength(targetPos - curPos) / activeDurSec;
			movData.mMovDir = Math::unitVector(targetPos - curPos);
			curPos = targetPos;
		}
		procAutoParentMovData.mMovDataList.push_back(movData);
	}
}

void AutoBody2dMovementSystem::finalizeToBeDeletedProcess(AutoBody2dMovementComponent* autoBody2dMovComp,
	Body2dComponent* body2dComp, EntityChildrenComponent* entityChildComp, Entity* entity, 
	EntityManager* entityManager, ProcessAutoSingleMovData& procSingleMovData, 
	ProcessAutoParentMovData& procParentMovData)
{
	body2dComp->setTransform(procSingleMovData.mOriginalTransform);
	body2dComp->setMainFixtureActive(false);
	if (procSingleMovData.mOriginalShapePtr.get()){
		body2dComp->setMainFixtureNewShape(procSingleMovData.mOriginalShapePtr.get());
		mBlockAllocator.Free(procSingleMovData.mOriginalShapePtr.release(), sizeof(b2PolygonShape));
	}
	
	body2dComp->setVelocity(0.f, 0.f, false);

	if (entityChildComp){
		Entity* parentComp = entityChildComp->getParent();
		Body2dComponent* parentBody2dComp = parentComp->getComp<Body2dComponent>();
		parentBody2dComp->setBaseSpeed(procParentMovData.mOriMovSpeed);
		//parentbody2dComp->setSteeringBehavior(procParentMovData.mOriSteeringBehaviorFlag);
	}
	autoBody2dMovComp->mIsMoving = false;
	autoBody2dMovComp->mRequestSystemToStop = false;
	if (procSingleMovData.mOnDoneFunc){
		(*procSingleMovData.mOnDoneFunc)(entity, entityManager);
		procSingleMovData.mOnDoneFunc = nullptr;
	}

	deleteProcess(autoBody2dMovComp);
	return;
}

void AutoBody2dMovementSystem::deleteProcess(AutoBody2dMovementComponent* autoBody2dComp)
{
	auto iter = getProcessAutoBody2dMapIter(autoBody2dComp);
	if (iter != mProcessAutoBody2dMap.end())
		mProcessAutoBody2dMap.erase(iter);
}

void AutoBody2dMovementSystem::cleanupEntityToBeRemoved(EntityManager* entityManager,
	Entity* entityToBeDeleted, sf::Time dt)
{
	AutoBody2dMovementComponent* autoBody2dComp = 
		entityToBeDeleted->getComp<AutoBody2dMovementComponent>();

	deleteProcess(autoBody2dComp);
}


bool AutoBody2dMovementSystem::isAutoBody2dMovCompAlreadyInProcess(
	AutoBody2dMovementComponent* autoBody2dMovComp) const
{
	return getProcessAutoBody2dMapIter(autoBody2dMovComp) != mProcessAutoBody2dMap.end();
}

AutoBody2dMovementSystem::ProcessAutoBody2dMap::iterator AutoBody2dMovementSystem::getProcessAutoBody2dMapIter(
	AutoBody2dMovementComponent* autoBody2dMovComp)
{
	auto findIter = mProcessAutoBody2dMap.find(autoBody2dMovComp);
	return findIter;
}

AutoBody2dMovementSystem::ProcessAutoBody2dMap::const_iterator AutoBody2dMovementSystem::getProcessAutoBody2dMapIter(
	AutoBody2dMovementComponent* autoBody2dMovComp) const
{
	auto findIter = mProcessAutoBody2dMap.find(autoBody2dMovComp);
	return findIter;
}
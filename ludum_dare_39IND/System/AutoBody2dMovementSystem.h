#pragma once
#include "SingleEntitySystem.h"
#include <unordered_map>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2BlockAllocator.h>
#include <Box2D\Collision\Shapes\b2Shape.h>
#include "../Component/AutoBody2dMovementComponent.h"

class Body2dComponent;
class VelocityComponent;
class EntityChildrenComponent;




struct MovData{
	/*for movement*/
	sf::Vector2f mMovDir;
	float mSpeedPerSec;
	/*for rotational*/
	float mDegreeIdentPerSec;

	

	sf::Time mActiveDur;
	sf::Time mDelayDur;
};

struct ProcessAutoSingleMovData{
	ProcessAutoSingleMovData();
	ProcessAutoSingleMovData(ProcessAutoSingleMovData& right);
	ProcessAutoSingleMovData& operator=(ProcessAutoSingleMovData& right);
	
	sf::Time mElapTimeSingleMov;
	std::size_t mCurSingleMovIndex;

	std::vector<MovData> mMovDataList;

	b2Transform mOriginalTransform;
	std::unique_ptr<b2Shape> mOriginalShapePtr;
	bool mIsProcessDone;

	const luabridge::LuaRef* mOnActiveFunc;
	const luabridge::LuaRef* mOnDelayFunc;
	const luabridge::LuaRef* mOnDoneFunc;
};

struct ProcessAutoParentMovData{
	sf::Time mElapTimeParentMov;
	std::size_t mCurParentMovIndex;

	std::vector<MovData> mMovDataList;
	float mOriMovSpeed;
	bool mOriSteeringBehaviorFlag;
	bool mIsProcessDone;
};

struct ProcessAutoMovementData{
	ProcessAutoSingleMovData mProcAutoSingleMovData;
	ProcessAutoParentMovData mProcAutoParentMovData;
	std::string mAutoMovSetName;
};

class AutoBody2dMovementSystem :public SingleEntitySystem<AutoBody2dMovementSystem>{
public:
	AutoBody2dMovementSystem();
	virtual ~AutoBody2dMovementSystem();
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
private:
	typedef std::unordered_map<AutoBody2dMovementComponent*, ProcessAutoMovementData> ProcessAutoBody2dMap;
private:
	void resumeProcessAutoSingleMovementData(AutoBody2dMovementComponent* autoBody2dMovComp,
		Body2dComponent* body2dComp,
		ProcessAutoSingleMovData& processAutoSingleMovData,
		const AutoMovementSetData& autoMovSetData, EntityManager* entityManager, sf::Time dt);

	void resumeProcessAutoParentMovementData(AutoBody2dMovementComponent* autoBody2dMovComp,
		Body2dComponent* body2dComp,
		EntityChildrenComponent* entityChildComp,
		ProcessAutoParentMovData& processAutoParentMovData, 
		const AutoMovementSetData& autoMovSetData, EntityManager* entityManager, sf::Time dt);


	void processRequestForEntry(Entity* entity,
		AutoBody2dMovementComponent* autoBody2dMovComp, 
		Body2dComponent* body2dComp);

	void processInitialSingleMovEntry(AutoBody2dMovementComponent* autoBody2dMovComp,
		Body2dComponent* body2dComp,
		ProcessAutoSingleMovData& processAutoSingleMovData,
		const AutoMovementSetData& autoMovSetData);

	void processInitialParentMovEntry(AutoBody2dMovementComponent* autoBody2dMovComp,
		Body2dComponent* body2dComp,
		EntityChildrenComponent* entityChildComp,
		ProcessAutoParentMovData& procAutoParentMovData,
		const AutoMovementSetData& autoMovSetData);

	bool isAutoBody2dMovCompAlreadyInProcess(AutoBody2dMovementComponent* autoBody2dMovComp) const;

	ProcessAutoBody2dMap::iterator getProcessAutoBody2dMapIter(
		AutoBody2dMovementComponent* autoBody2dMovComp);

	ProcessAutoBody2dMap::const_iterator getProcessAutoBody2dMapIter(
		AutoBody2dMovementComponent* autoBody2dMovComp) const;

	void cleanupEntityToBeRemoved(EntityManager* entityManager,
		Entity* entityToBeDeleted, sf::Time dt) override;
	
	void finalizeToBeDeletedProcess(AutoBody2dMovementComponent* autoBody2dMovComp, 
		Body2dComponent* body2dComp, 
		EntityChildrenComponent* entityChildComp,
		Entity* entity, 
		EntityManager* entityManager, 
		ProcessAutoSingleMovData& procSingleMovData, 
		ProcessAutoParentMovData& procParentMovData);
	void deleteProcess(AutoBody2dMovementComponent* autoBody2dMovComp);
private:
	ProcessAutoBody2dMap mProcessAutoBody2dMap;
	b2BlockAllocator mBlockAllocator;
};


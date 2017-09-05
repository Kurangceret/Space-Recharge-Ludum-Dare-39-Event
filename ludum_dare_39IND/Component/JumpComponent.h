#pragma once
#include "Component.h"
#include <vector>
#include <SFML/System/Time.hpp>
class Entity;

class JumpStateEvent;
class JumpSystem;
class ComponentArranger;

/*struct JumpData{
	typedef std::unique_ptr<JumpData> Ptr;
	sf::Vector2f mJumpDir;
	sf::Vector2f mGravityDir;
	sf::Time mCurTimeJumpVeloHold;
	sf::Vector2f mCurJumpVeloHold;
};

struct RequestJumpData{
	typedef std::unique_ptr<RequestJumpData> Ptr;
	RequestJumpData();
	
	float mJumpDegree;
	sf::Vector2f mFloorDir;
	//this is used when floor dir x/y is empty, should only be value 1 positive or 1 negative
	//used so we can know to jump "up" or "down" based on the direction of the floor dir
	sf::Vector2f mGravityDirIdent;

	float mInitialJumpVelo;
	sf::Time mCurTimeJumpVeloHold;
	float mJumpVeloHoldPercent;

};*/

class JumpComponent :	public Component{
public:
	typedef std::vector<const Entity*> GroundEntities;
	friend JumpStateEvent;
	friend JumpSystem;
	friend ComponentArranger;
public:
	JumpComponent(Entity* entity);
	virtual ~JumpComponent();

	
	void startJumpImpulse(float jumpImpulse = 0.f);
	


	void prolongJump();
	bool isOnGround() const;
	//this function uses the index counting, 
	//rather than checking at player's sensor/fixtiures
	bool isJumping() const;
	void resetJumpCount();

	void detachGroundEntity(const Entity* entity);
	void attachGroundEntity(const Entity* entity);

	int getMaxNumOfJumpLimit() const;
	void setCurNumberOfJumpLimit(int newVal);
private:
	GroundEntities::const_iterator getGroundIter(const Entity* entity) const;
private:
	int mMaxNumberOfJumpLimit;
	int mCurNumberOfJumpLimit;
	int mCurrentJumpCount;
	//make sure this container is filled with entity detected on
	//entity's imaginary feet/foot
	float mJumpImpulse;
	
	/*float mMaxIncJumpImpulse;
	float mCurIncJumpImpulse;
	float mIncJumpImpulsePerSec;*/
	/*float mMaxIncJumpPos;
	float mCurIncJumpPos;
	float mIncJumpPosPerSec;*/

	sf::Time mMaxTimeJumpVeloHold;
	sf::Time mCurTimeJumpVeloHold;
	float mJumpVeloHoldVal;

	bool mWillProlongJump;


	GroundEntities mGroundEntities;
};


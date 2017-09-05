#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <unordered_map>
#include <Box2D\Collision\Shapes\b2Shape.h>
#include <string>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class AutoBody2dMovementSystem;

struct AutoSingleMoveData{
	
	//applied to current pos
	sf::Vector2f mOffsetPos;
	//applied to current angle
	float mOffsetDegree;

	sf::Time mDelayDur;
	sf::Time mActiveDur;
};

struct AutoParentMoveData{
	
	//applied to parent current pos
	sf::Vector2f mOffsetPos;
	//applied to parent current degree
	float mOffsetDegree;
	//duration before we start to add offset pos & 
	//offset degree to the parent
	sf::Time mDelayDur;
	
	//duration start
	sf::Time mActiveDur;
		
};

struct AutoMovementSetData{
	AutoMovementSetData();
	AutoMovementSetData(AutoMovementSetData& autoMovementSetData);

	//if this is set to true,
	//mStartOffsetPos is no longer offset to current pos
	//instead it replace the cur pos
	bool mIsStartingPosAbsolute;
	bool mUseStartingPos;
	sf::Vector2f mStartOffsetPos;

	bool mUseStartingDegree;
	float mStartOffsetDegree;

	bool mUseRelativeToBaseMovDir;
	float mDegreeRelToBaseMovDir;
	sf::Vector2f mPosRelToBaseMovDir;

	std::vector<AutoSingleMoveData> mSingleMoveDataList;
	std::vector<AutoParentMoveData> mParentMoveDataList;

	std::unique_ptr<b2Shape> mBaseShapePtr;

	std::unique_ptr<luabridge::LuaRef> mOnDelayMoveFunc;
	std::unique_ptr<luabridge::LuaRef> mOnActiveMoveFunc;
	std::unique_ptr<luabridge::LuaRef> mOnDoneMoveFunc;

	sf::Time mVulnerableDur;
	sf::Time mRecoveryDur;

	bool mReverseDataForNegativeXBaseMovDir;

	bool mUseLatestDegreeForOffset;
	sf::Vector2f mLatestDegreeOffsetPos;
};



class ComponentArranger;
//class uses for mostly fast paced but flexible body movement
//that is controlled by a pre determined data
class AutoBody2dMovementComponent :	public Component{
public:
	friend class AutoBody2dMovementSystem;
	friend ComponentArranger;
public:
	struct CustomData{
		CustomData();
		bool mUseCustomData;
		bool mReverseDataForPositiveBaseMovDir;
	};
public:
	AutoBody2dMovementComponent(Entity* entity);
	virtual ~AutoBody2dMovementComponent();
	
	void startMovement(const sf::Vector2f& autoMovDir,
		const std::string& autoMovementName, const CustomData& customData = CustomData());

	void stopCurMovement();

	bool isMovingNow() const;
	std::string getCurrentMovementName() const;
	sf::Vector2f getCurAutoMovBaseDir() const;

	const AutoMovementSetData& 
		getConstAutoMovementSetData(const std::string& autoMovementName) const;
	
private:
	typedef std::unordered_map<std::string, AutoMovementSetData> AutoMovementMap;

	AutoMovementSetData& getAutoMovementSetData(const std::string& autoMovementName);
	AutoMovementMap::const_iterator getAutoMovementIter(const std::string& autoMovementName) const;
	AutoMovementMap::iterator getAutoMovementIter(const std::string& autoMovementName);
private:
	//if set to true, AutoBody2dMovementSystem will take care of its update
	bool mIsRequestingSystem;
	//if set to true, AutoBody2dMovementSystem will abort the update
	bool mRequestSystemToStop;
	//
	bool mIsMoving;

	AutoMovementMap mAutoMovementList;
	sf::Vector2f mCurAutoMovBaseDir;
	std::string mCurMovementName;
	//can use this one to prevent multiple application to touched entities
	std::vector<const Entity*> mTouchedEntities;

	CustomData mCustomData;
};


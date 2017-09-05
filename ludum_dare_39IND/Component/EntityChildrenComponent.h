#pragma once
#include "Component.h"
#include <vector>
#include <SFML/System/Vector2.hpp>

class EntityParentComponent;
class EntityManager;
class ComponentArranger;




class ChildPosType{
public:
	typedef std::unique_ptr<ChildPosType> ChildPosTypePtr;
public:
	ChildPosType() = default;
	~ChildPosType() = default;

	/*virtual sf::Vector2f getPosRelativeToParent(Entity* thisEntity, Entity* entParent) const = 0;
	virtual sf::Vector2f getFacingDirWithParent(Entity* thisEntity, Entity* entParent) const = 0;*/
private:
};

class AbsoluteChildPosType : public ChildPosType{
public:
	typedef std::unique_ptr<AbsoluteChildPosType> AbsoluteChildPosTypePtr;
	friend ComponentArranger;
public:
	enum AbsRotationalType {
		OffsetToCurDir,
		AbsToCurDir,
		NoneToCurDir,
		AbsRotationalTypeCount
	};
public:
	AbsoluteChildPosType();
	virtual ~AbsoluteChildPosType() = default;

	

private:
	sf::Vector2f mAbsolutePos;
	AbsRotationalType mRotationalType;
};


class RelativeChildPosType : public ChildPosType{
public:
	typedef std::unique_ptr<RelativeChildPosType> RelativeChildPosTypePtr;
	friend ComponentArranger;
public:
	RelativeChildPosType();
	virtual ~RelativeChildPosType() = default;

	
	
private:
	float mDegreeRelToParentDir;
	sf::Vector2f mDistToParentOrig;
	sf::Vector2f mOpossiteDist;
};

class EntityChildrenComponent : public Component{
public:
	friend EntityParentComponent;
	friend ComponentArranger;
public:
	EntityChildrenComponent(Entity* entity);
	virtual ~EntityChildrenComponent();

	Entity* getParent() const;
	
private:
	Entity* mParent;
	ChildPosType::ChildPosTypePtr mChildPosTypePtr;
};


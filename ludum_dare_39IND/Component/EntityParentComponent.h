#pragma once
#include "Component.h"
#include <vector>

class Entity;
class EntityChildrenComponent;
class ComponentArranger;
class EntityManager;
struct lua_State;
/*class EntityParentComponent :	public Component{
public:
	friend EntityChildrenComponent;
	friend ComponentArranger;
public:
	EntityParentComponent(Entity* entity);
	virtual ~EntityParentComponent();

	Entity* getParent();
private:
	Entity* mParent;
};*/

/*struct ChildPosType{

};*/

/*struct EntityChildData{
	EntityChildData();
	EntityChildData(Entity* childEntity);
	Entity* mChildEntity;
	bool mAffectedByParentRotation;
	//if set to true, child with autobody2dcomponent
	//will be able to move the parent's transform
	bool mAllowChildToAutoParent;
};*/

class EntityParentComponent : public Component {
public:
	EntityParentComponent(Entity* entity);
	virtual ~EntityParentComponent();


	void attachChild(Entity* childEntity);
	void removeChild(Entity* childEntity);
	void removeRemoveableChild(EntityManager* entityManager);

	void insertNewChild(Entity* childEntity);
	bool isCategoryInsideChildren(std::size_t category) const;

	Entity* getChild(int index) const;
	Entity* getChild(Entity* childToGet) const;
	//const EntityChildData& getChildData(Entity* childToGet) const;
	//void setChildAffectedByParentRotation(Entity* child, bool flag);
private:
	//EntityChildData& getEditableChildData(Entity* childToGet);
private:
	std::vector<Entity*> mChildren;
};
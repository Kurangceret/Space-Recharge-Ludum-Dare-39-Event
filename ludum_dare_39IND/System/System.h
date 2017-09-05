#pragma once

#include "BaseSystem.h"

class SystemManager;

template<class T>
class System : public BaseSystem{
public:
	typedef std::unique_ptr<System> Ptr;
	friend SystemManager;

public:
	virtual ~System() = default;
	
	//normal update that began the 
	//update cycle within the eligible entity
	//virtual void update(sf::Time dt) override;

	//whether entity is updateable by this system
	//virtual bool isEntityEligible(Entity* entity) const;
	//addEntity will automatically call entityEligible func
	//void addEntity(Entity* entity);
		
protected:
	System() = default;

	/*virtual void processEntity(Entity* entity, sf::Time dt) = 0;
	void pushRequiredComponent(std::size_t getComponentIdentifier);*/
	//increase R
	static RefID getRefID(){
		static RefID refId = mRefIDCounter++;
		return refId;
	}
private:
	
	
};



/*template<class T>
System<T>::RefID System<T>::getRefID()
{
	static RefID refId = mRefIDCounter++;
	return refId;
}*/

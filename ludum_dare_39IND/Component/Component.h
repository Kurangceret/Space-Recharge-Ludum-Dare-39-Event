#pragma once
#include <memory>

class Entity;

class Component{
public:
	typedef std::unique_ptr<Component> Ptr;
public:
	virtual ~Component();

	std::size_t mIdentifier;

	Entity* getOwnerEntity() const;
protected:
	Component(Entity* entity);
	Component(Entity* entity, std::size_t getCompIdentifier);
private:
	Entity* mOwnerEntity;
};


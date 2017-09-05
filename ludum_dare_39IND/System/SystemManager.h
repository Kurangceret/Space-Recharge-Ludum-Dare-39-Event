#pragma once
#include "System.h"
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Entity;
class EntityManager;
class FourTree;

class SystemManager{
public:
	SystemManager();
	~SystemManager();

	template<class T, typename ...Args>
	void addSystem(Args&& ... args);

	template<class T>
	void addSystem(std::unique_ptr<System<T>> ptr);

	template<class T>
	void update(EntityManager* entityManager, sf::Time dt);

	template<class T>
	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states);

	template<class T>
	void handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event);
	//void updateAll(sf::Time dt);

	void putEntityInSystems(std::shared_ptr<Entity>& entity);

	void setSystemsBoundRect(const sf::FloatRect& boundRect);
private:
	template<class T>
	BaseSystem* getSystem();

private:
	std::unordered_map<BaseSystem::RefID, BaseSystem::Ptr> mBaseSystems;
};

template <class T, typename ...Args>
void SystemManager::addSystem(Args&& ... args)
{
	std::unique_ptr<System<T>> ptr(new T(std::forward<Args>(args)...));
	addSystem<T>(std::move(ptr));
}

template<class T>
void SystemManager::addSystem(std::unique_ptr<System<T>> ptr)
{
	mBaseSystems.insert(std::make_pair(System<T>::getRefID(), std::move(ptr)));
}

template<class T>
void SystemManager::update(EntityManager* entityManager, sf::Time dt)
{
	BaseSystem* system = getSystem<T>();
	system->update(entityManager, dt);
}

template<class T>
void SystemManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states)
{
	BaseSystem* system = getSystem<T>();
	system->draw(renderTarget, states);
}

template<class T>
void SystemManager::handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event)
{
	BaseSystem* system = getSystem<T>();
	system->handleEvent(renderWindow, event);
}

template<class T>
BaseSystem* SystemManager::getSystem()
{
	auto iter = mBaseSystems.find(System<T>::getRefID());
	assert(iter != mBaseSystems.end());

	return iter->second.get();
}

/*template<class T>
std::shared_ptr<T> SystemManager::getSystem()
{
	auto iter = mBaseSystems.find(System<T>::getRefID());
	assert(iter != mBaseSystems.end());

	return std::shared_ptr<T>(std::static_pointer_cast<T>(iter->second.get()));
}*/




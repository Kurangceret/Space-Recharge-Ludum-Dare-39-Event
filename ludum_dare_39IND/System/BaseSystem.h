#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <SFML/Window/Event.hpp>
class Entity;
class EntityManager;


class BaseSystem{
public:
	typedef std::unique_ptr<BaseSystem> Ptr;
	typedef std::size_t RefID;
public:
	virtual ~BaseSystem();


	virtual void handleEvent(const sf::RenderWindow& renderWindow, const sf::Event&);

	virtual void update(EntityManager* entityManager, sf::Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states);

	//this function will do the checking of the entity eligibilty
	virtual void addEntity(std::shared_ptr<Entity>& entity) = 0;

	void setBoundRect(const sf::FloatRect& boundRect);
protected:
	BaseSystem();
	//this function will be called before the main update function starts
	virtual bool initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt);
	//this function will be called right after the main update function is finished
	virtual void finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt);

	void setAdhereToQuadTree(bool flag);
	void setAdhereToBoundRect(bool flag);

	bool isAdheringToQuadTree() const;
	bool isAdheringToBoundRect() const;

	void setBoundAdjuster(const sf::Vector2f& boundAdjusterPos);

	bool isEntityRestrictedByBoundRect(Entity* entity) const;
private:
	sf::FloatRect getBoundRect(bool withBoundAdjuster = true) const;
protected:
	static RefID mRefIDCounter;

	bool mAdhereToQuadTree;
	bool mAdhereToBoundRect;

	sf::FloatRect mBoundRect;
	sf::Vector2f mBoundAdjusterPos;
};


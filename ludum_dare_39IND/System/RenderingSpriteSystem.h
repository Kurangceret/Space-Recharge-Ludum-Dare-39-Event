#pragma once
#include "SingleEntitySystem.h"
#include <vector>
#include <queue>
#include <map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../CustomBox2D/SfDebugB2Draw.h"

class SpriteComponent;

class RenderingSpriteSystem :
	public SingleEntitySystem<RenderingSpriteSystem>
{
public:
	RenderingSpriteSystem(sf::RenderTarget& renderTarget);
	virtual ~RenderingSpriteSystem();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
protected:
	virtual void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt);

	bool initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
	void finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
private:
	class SpriteData{
	public:
		SpriteData(SpriteComponent* spriteComp, const sf::Vector2f&, 
			sf::RenderStates, std::size_t);
		
		SpriteComponent* mSpriteComp;
		sf::Vector2f mSpriteWorldPos;
		sf::RenderStates mSpriteStates;
		std::size_t mSpriteZIndex;
	};

	struct SpritePriorityChecker{
		bool operator()(SpriteData& left, SpriteData& right) const;
	};

private:
	std::priority_queue<RenderingSpriteSystem::SpriteData, 
		std::vector<RenderingSpriteSystem::SpriteData>,
		RenderingSpriteSystem::SpritePriorityChecker> mPrioritySpriteList;

	std::map<std::size_t, std::vector<SpriteData>> mSpriteCompList;
	SfDebugB2Draw mDebug2dDraw;
	sf::Font mFont;
	sf::Text mTextID;

	sf::RenderTarget& mRenderTarget;

	sf::FloatRect mCurWindowViewRect;
};


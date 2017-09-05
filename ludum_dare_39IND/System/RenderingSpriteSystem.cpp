#include "RenderingSpriteSystem.h"
#include "../Entity/Entity.h"
#include "../Component/SpriteComponent.h"

#include "../Helper/Utility.h"

#include <SFML\Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../Component/Body2dComponent.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Common\b2Math.h>
#include <Box2D\Common\b2Settings.h>
#include <Box2D\Collision\Shapes\b2EdgeShape.h>
#include "../Component/AutomaticPathComponent.h"

RenderingSpriteSystem::SpriteData::SpriteData(SpriteComponent* spriteComp, 
	const sf::Vector2f& pos, sf::RenderStates otherStates, std::size_t zIndex)
:mSpriteComp(spriteComp),
mSpriteWorldPos(pos),
mSpriteStates(otherStates),
mSpriteZIndex(zIndex)
{
	
}


bool RenderingSpriteSystem::SpritePriorityChecker::operator()
	(SpriteData& left, SpriteData& right) const
{
	return (left.mSpriteZIndex > right.mSpriteZIndex) || ((left.mSpriteWorldPos.y > right.mSpriteWorldPos.y) ||
		(left.mSpriteWorldPos.y == right.mSpriteWorldPos.y &&
		left.mSpriteWorldPos.x > right.mSpriteWorldPos.x));
}

RenderingSpriteSystem::RenderingSpriteSystem(sf::RenderTarget& renderTarget)
:mDebug2dDraw(renderTarget),
mRenderTarget(renderTarget)
{
	mFont.loadFromFile("Media/Fonts/Sansation.ttf");
	mTextID.setColor(sf::Color(255, 255, 255, 255));
	mTextID.setCharacterSize(12);
	mTextID.setFont(mFont);
	pushRequiredComponent(ComponentIdentifier::SpriteComponent);
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);

}

RenderingSpriteSystem::~RenderingSpriteSystem()
{
	
}



void RenderingSpriteSystem::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& pair : mSpriteCompList){
		for (auto& spriteData : pair.second){
			/*sf::RenderStates states(sf::RenderStates::Default);
			states.transform.translate(spriteData.mSpriteWorldPos);*/
			sf::RenderStates curSpriteStates = spriteData.mSpriteStates;
			curSpriteStates.transform *= states.transform;

			Entity* entity = spriteData.mSpriteComp->getOwnerEntity();

			if (spriteData.mSpriteComp->mElapsedPeriod 
				>= spriteData.mSpriteComp->mPeriodBlink)
			{
				spriteData.mSpriteComp->mElapsedPeriod 
					-= spriteData.mSpriteComp->mPeriodBlink;
				continue;
			}
			
			
			Utility::drawFloatToInt(target, curSpriteStates.transform, 
				spriteData.mSpriteComp->getSprite());
			
			if (spriteData.mSpriteComp->mDrawEntName){
				mTextID.setString(std::to_string(entity->getId()));
				target.draw(mTextID, curSpriteStates.transform);
			}

			/*auto automaticPathComp = entity->nonCreateComp<AutomaticPathComponent>();
			if (automaticPathComp)
				automaticPathComp->drawAutomaticPaths(target);*/

			auto body2dComp = entity->nonCreateComp<Body2dComponent>();
			if (!body2dComp || !spriteData.mSpriteComp->mDrawFixtures){
				continue;
			}
			b2Transform bodyTransform = body2dComp->getTransform();
			for (const auto& fixtureLogic : body2dComp->mFixtureLogicList){
				if (!fixtureLogic.mIsActive && !spriteData.mSpriteComp->mDrawNonActiveFixtures)
					continue;
				b2Shape* shape = fixtureLogic.mFixture->GetShape();
				if (shape->GetType() == b2Shape::e_polygon){
					b2PolygonShape* polygonShape = static_cast<b2PolygonShape*>(shape);

					b2Vec2 vertices[b2_maxPolygonVertices];

					for (int i = 0; i < b2_maxPolygonVertices; i++){
						vertices[i] = b2Mul(bodyTransform, polygonShape->m_vertices[i]);
					}

					mDebug2dDraw.DrawPolygon(vertices,
						polygonShape->m_count, 
						Utility::sfColorToBox2dColor(fixtureLogic.mDrawColor));
				}
				else if (shape->GetType() == b2Shape::e_circle){
					b2CircleShape* circleShape = static_cast<b2CircleShape*>(shape);
					mDebug2dDraw.DrawSolidCircle(b2Mul(bodyTransform, circleShape->m_p), 
						circleShape->m_radius, b2Vec2(0.f, 0.f), 
						Utility::sfColorToBox2dColor(fixtureLogic.mDrawColor));
				}
				else if (shape->GetType() == b2Shape::e_edge){
					b2EdgeShape* edgeShape = static_cast<b2EdgeShape*>(shape);
					
					mDebug2dDraw.DrawSegment(b2Mul(bodyTransform, edgeShape->m_vertex1), 
						b2Mul(bodyTransform, edgeShape->m_vertex2),
						Utility::sfColorToBox2dColor(fixtureLogic.mDrawColor));
				}
			}


			
			//target.draw(spriteData.mSpriteComp->getSprite(), curSpriteStates);

			
			
		}
	}
}

bool RenderingSpriteSystem::initiatingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	const auto& curView = mRenderTarget.getView();
	const sf::Vector2f& viewCenter = curView.getCenter();
	const sf::Vector2f& viewSize = curView.getSize();

	mCurWindowViewRect.width = viewSize.x + 200.f;
	mCurWindowViewRect.height = viewSize.y + 200.f;
	mCurWindowViewRect.left = viewCenter.x - mCurWindowViewRect.width / 2;
	mCurWindowViewRect.top = viewCenter.y - mCurWindowViewRect.height / 2;
	mSpriteCompList.clear();
	return true;
}

void RenderingSpriteSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	
	
	Body2dComponent* body2dComp = entity->getComp<Body2dComponent>();


	sf::FloatRect curEntRect = Utility::b2AABBToSfFloatRect(
		body2dComp->getMainFixture()->GetAABB(0));

	if (!curEntRect.intersects(mCurWindowViewRect))
		return;

	/*auto& aabb = body2dComp->getMainFixture()->GetAABB(0);
	aabb.*/

	SpriteComponent* spriteComp = entity->getComp<SpriteComponent>();
	spriteComp->updateBlinkStatus(dt);
	std::size_t zIndex = entity->getZIndex();

	sf::RenderStates spriteStates;
	sf::Vector2f spriteWorldPos;
	
	spriteStates = body2dComp->getSfTransform(false);
	spriteStates.transform.translate(spriteComp->getSpriteWorldOrigin());

	spriteWorldPos = spriteStates.transform.transformPoint(sf::Vector2f());
	
	
	/*mPrioritySpriteList.push(SpriteData(spriteComp, 
		transformableComp->getWorldPosition(false), zIndex));*/
	mSpriteCompList[zIndex].push_back(SpriteData(spriteComp, 
		spriteWorldPos, spriteStates, zIndex));
}

void RenderingSpriteSystem::finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	//std::lexicographical_compare()
	/*for (auto& pair : mSpriteCompList){
		
		std::sort(pair.second.begin(), pair.second.end()
			, [](SpriteData& leftSpriteData, SpriteData& rightSpriteData) -> bool
		{
			
			
			return 
				((leftSpriteData.mSpriteWorldPos.y < rightSpriteData.mSpriteWorldPos.y) ||
				(leftSpriteData.mSpriteWorldPos.y == rightSpriteData.mSpriteWorldPos.y &&
				leftSpriteData.mSpriteWorldPos.x < rightSpriteData.mSpriteWorldPos.x));

		});
	}*/
}
#include "SpriteComponent.h"
#include "ComponentIdentifier.h"
#include "../Entity/Entity.h"
#include <SFML/Graphics/Texture.hpp>

SpriteComponent::SpriteComponent(Entity* entity)
:Component(entity, ComponentIdentifier::SpriteComponent),
mBlinkDuration(sf::seconds(0.90f)),
mPeriodBlink(sf::seconds(0.15f)),
mCurBlinkDur(sf::Time::Zero),
mElapsedPeriod(sf::Time::Zero),
mDrawEntName(false),
mDrawFixtures(false),
mDrawNonActiveFixtures(false)
{
	
	//std::size_t i = ComponentIdentifier::SpriteComponent | ComponentIdentifier::PositionComponent;
}


SpriteComponent::~SpriteComponent()
{
}


void SpriteComponent::beganBlink()
{
	mElapsedPeriod = sf::Time::Zero;
	//mBlinkDuration = sf::seconds(1.2f);
	mCurBlinkDur = mBlinkDuration;
	//mPeriodBlink = sf::seconds(0.1f);
}

void SpriteComponent::updateBlinkStatus(sf::Time dt)
{
	if (mCurBlinkDur.asSeconds() <= 0.0f)
		return;
	mCurBlinkDur -= dt;
	mElapsedPeriod += dt;
}

void SpriteComponent::setTexture(const sf::Texture* texture)
{
	mSprite.setTexture(*texture);
}

void SpriteComponent::setTextureRect(const sf::IntRect& intRect)
{
	mSprite.setTextureRect(intRect);
}


void SpriteComponent::flip_sprite_horizontally()
{
	sf::IntRect ori_rect = mSprite.getTextureRect();

	//ori_rect.
}

void SpriteComponent::setSpriteWorldOrigin(const sf::Vector2f& ori)
{
	setSpriteWorldOrigin(ori.x, ori.y);
}

void SpriteComponent::setSpriteWorldOrigin(float x, float y)
{
	mSpriteWorldOrigin.x = x;
	mSpriteWorldOrigin.y = y;
}

void SpriteComponent::setSpriteWorldOriginScript(float x, float y)
{
	setSpriteWorldOrigin(x, y);
}

void SpriteComponent::setTextureRectOriginScript(int xOrigin, int yOrigin)
{
	sf::IntRect intRect = mSprite.getTextureRect();
	intRect.left = xOrigin;
	intRect.top = yOrigin;
	mSprite.setTextureRect(intRect);
}

const sf::Sprite& SpriteComponent::getSprite() const
{
	return mSprite;
}


void SpriteComponent::setSpriteColor(const sf::Color& color)
{
	mSprite.setColor(color);
}

sf::Color SpriteComponent::getSpriteColor() const
{
	return mSprite.getColor();
}

sf::Vector2f SpriteComponent::getSpriteWorldOrigin() const
{
	return mSpriteWorldOrigin;
}

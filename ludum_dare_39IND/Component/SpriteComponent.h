#pragma once
#include "Component.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>


class ComponentArranger;
class AnimationComponent;
class RenderingSpriteSystem;

class SpriteComponent :	public Component{
public:
	//friend RenderingSystem;
	friend ComponentArranger;
	friend AnimationComponent;
	friend RenderingSpriteSystem;
public:
	SpriteComponent(Entity* entity);
	virtual ~SpriteComponent();
		

	void setTexture(const sf::Texture* texture);
	void setTextureRect(const sf::IntRect& intRect);


	void flip_sprite_horizontally();

	void setSpriteWorldOrigin(const sf::Vector2f& ori);
	void setSpriteWorldOrigin(float x, float y);

	void setSpriteWorldOriginScript(float x, float y);

	void setTextureRectOriginScript(int xOrigin, int yOrigin);

	const sf::Sprite& getSprite() const;

	void beganBlink();
	void updateBlinkStatus(sf::Time dt);
	
	void setSpriteColor(const sf::Color& color);
	sf::Color getSpriteColor() const;

	sf::Vector2f getSpriteWorldOrigin() const;
private:
	sf::Time mBlinkDuration;
	sf::Time mPeriodBlink;

	sf::Time mCurBlinkDur;
	sf::Time mElapsedPeriod;


	sf::Vector2f mSpriteWorldOrigin;
	sf::Sprite mSprite;

	bool mDrawFixtures;
	bool mDrawNonActiveFixtures;
	bool mDrawEntName;
	
};


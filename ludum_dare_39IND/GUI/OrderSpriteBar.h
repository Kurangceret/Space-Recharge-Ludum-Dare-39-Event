#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#

class OrderSpriteBar{
public:
	OrderSpriteBar();
	~OrderSpriteBar();


	void setSpriteTexture(const sf::Texture& texture);
	void update(std::size_t num, sf::Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states);

private:
	std::size_t mCurNum;
	sf::Sprite mSprite;
};


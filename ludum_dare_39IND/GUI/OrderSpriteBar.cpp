#include "OrderSpriteBar.h"


OrderSpriteBar::OrderSpriteBar()
:mCurNum(0)
{
}


OrderSpriteBar::~OrderSpriteBar()
{
}



void OrderSpriteBar::setSpriteTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}


void OrderSpriteBar::update(std::size_t num, sf::Time dt)
{
	mCurNum = num;
}

void OrderSpriteBar::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	for (int i = 0; i < mCurNum; i++){
		target.draw(mSprite, states);

		states.transform.translate((mSprite.getTextureRect().width) + 3.f, 0.f);
	}
}
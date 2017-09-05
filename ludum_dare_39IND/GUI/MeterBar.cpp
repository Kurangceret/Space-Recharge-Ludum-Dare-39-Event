#include "MeterBar.h"
#include "../Helper/Constant.h"

const float meterBarWidth = 400.f;
const float meterBarHeight = 25.f;
const float chSize = meterBarHeight * 0.75f;

MeterBar::MeterBar(const sf::Color& barColor)
{
	mOuterRect.setFillColor(sf::Color::Transparent);
	mOuterRect.setOutlineThickness(1.f);
	mOuterRect.setOutlineColor(sf::Color::Blue);
	mOuterRect.setSize(sf::Vector2f(meterBarWidth, meterBarHeight));

	mFillRect.setFillColor(barColor);
	mFillRect.setSize(sf::Vector2f(meterBarWidth, meterBarHeight));

	mFont.loadFromFile(Constant::fontDir + "Sansation.ttf");

	mText.setFont(mFont);
	mText.setCharacterSize(chSize);

}


MeterBar::~MeterBar()
{
}

void MeterBar::setSize(const sf::Vector2f& size)
{
	mOuterRect.setSize(size);
	mFillRect.setSize(size);
}

void MeterBar::setSize(float width, float height)
{
	setSize(sf::Vector2f(width, height));
}


void MeterBar::drawMeter(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(mOuterRect, states);
	target.draw(mFillRect, states);

	states.transform.translate((meterBarWidth / 2.f) - (chSize * 1.f), 0.f);
	target.draw(mText, states);
}


void MeterBar::update(float curValue, float maxValue, sf::Time dt)
{	
	mText.setString(std::to_string(static_cast<int>(curValue)) + "/" + std::to_string(static_cast<int>(maxValue)));
	mFillRect.setSize(sf::Vector2f(mOuterRect.getSize().x *
		(curValue / maxValue), mOuterRect.getSize().y));
}
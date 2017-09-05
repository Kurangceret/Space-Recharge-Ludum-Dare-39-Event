#include "CircleBar.h"


const float finalRadius = 24.f;

CircleBar::CircleBar(const sf::Color& color)
:mColor(color)
{
}


CircleBar::~CircleBar()
{
}

void CircleBar::update(std::size_t circleNum, sf::Time dt)
{
	while (mCircleList.size() < circleNum)
		addNewCircle();

	while (mCircleList.size() > circleNum)
		mCircleList.erase(mCircleList.begin() + (mCircleList.size() - 1));
}

void CircleBar::drawCircles(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& circle : mCircleList){
		target.draw(circle, states);
		
		states.transform.translate((circle.getRadius() * 2.f) + 5.f, 0.f);
	}
}

void CircleBar::addNewCircle()
{
	sf::CircleShape circleShape;
	circleShape.setFillColor(mColor);
	circleShape.setRadius(finalRadius);
	circleShape.setOutlineThickness(0.f);

	mCircleList.push_back(circleShape);
}
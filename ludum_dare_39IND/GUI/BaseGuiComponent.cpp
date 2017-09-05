#include "BaseGuiComponent.h"


BaseGuiComponent::BaseGuiComponent()
:mIsHidden(false),
mIsRemoved(false)
{
}


BaseGuiComponent::~BaseGuiComponent()
{
}


void BaseGuiComponent::handleMouseEvent(const sf::Event& event, const sf::Vector2f& mousePosRelativeToWindow)
{
	//sf::Vector2f mousePosRelativeToWindow(sf::Mouse::getPosition(renderWindow));
	if (isHidden())
		return;

	if (event.type == sf::Event::MouseButtonPressed)
		handleOnClick(event.mouseButton.button, mousePosRelativeToWindow);
	else
		handleMouseHover(mousePosRelativeToWindow);
}

void BaseGuiComponent::setPositionOnWindow(const sf::Vector2f& posRelativeToWindow)
{
	mPosRelativeToWindow = posRelativeToWindow;
}

void BaseGuiComponent::handleMouseHover(const sf::Vector2f& mousePosRelativeToWindow)
{

}

void BaseGuiComponent::pushDrawableObj(const sf::Drawable& drawable)
{
	mDrawableGuiList.push_back(&drawable);
}

void BaseGuiComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	if (isHidden())
		return;

	finalizeDrawableGuiList();
	states.transform.translate(mPosRelativeToWindow);
	for (auto& drawable : mDrawableGuiList)
		target.draw(*drawable, states);

	mDrawableGuiList.clear();
}

void BaseGuiComponent::update(sf::RenderWindow& renderWindow, sf::Time dt)
{

}

void BaseGuiComponent::setIsRemoved(bool flag)
{
	mIsRemoved = flag;
}

void BaseGuiComponent::setIsHidden(bool flag)
{
	mIsHidden = flag;
}

bool BaseGuiComponent::isRemoved() const
{
	return mIsRemoved;
}

bool BaseGuiComponent::isHidden() const
{
	return mIsHidden;
}

sf::Vector2f BaseGuiComponent::getPosRelativeToWindow() const
{
	return mPosRelativeToWindow;
}

/*void BaseGuiComponent::setSpriteTexture(const sf::Texture& texture, const sf::IntRect& intRect)
{
	mGuiSprite.setTexture(texture);
	if (intRect != sf::IntRect())
		mGuiSprite.setTextureRect(intRect);

	mGuiSprite.setOrigin(mGuiSprite.getTextureRect().width / 2.f, 
		mGuiSprite.getTextureRect().height / 2.f);
}
void BaseGuiComponent::setSpriteOrigin(const sf::Vector2f& origin)
{
	mGuiSprite.setOrigin(origin);
}*/
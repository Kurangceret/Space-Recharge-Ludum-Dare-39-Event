#include "GuiButton.h"


GuiButton::GuiButton(BaseAction::Ptr baseAction)
:mBaseAction(std::move(baseAction)),
mCharacterSize(12),
mLimitSizePerLine(480),
mDrawTooltipForNextFrame(false),
mSpacingValInTooltipGui(10.f)
{
	mFont.loadFromFile("Media/Fonts/Sansation.ttf");
	mTooltipDrawableText.setCharacterSize(mCharacterSize);
	mTooltipDrawableText.setFont(mFont);

	mTooltipRectShape.setFillColor(sf::Color(0.f, 0.f, 0.f, 155.f));
}


GuiButton::~GuiButton()
{
}

void GuiButton::update(sf::RenderWindow& renderWindow, sf::Time dt)
{
	if (mTooltipDrawableText.getString().isEmpty())
		return;

	sf::Vector2f guiPosRelative = getPosRelativeToWindow();

	sf::FloatRect buttonGlobalBound = mGuiButtonSprite.getGlobalBounds();
	buttonGlobalBound.left += guiPosRelative.x;
	buttonGlobalBound.top += guiPosRelative.y;

	sf::Vector2f mousePosRelativeToWindow =
		sf::Vector2f(sf::Mouse::getPosition(renderWindow));

	if (buttonGlobalBound.contains(mousePosRelativeToWindow)){

		mTooltipRectShape.setPosition(sf::Vector2f(0.f, -mTooltipRectShape.getSize().y));
		mTooltipDrawableText.setPosition(mTooltipRectShape.getPosition() + sf::Vector2f(mSpacingValInTooltipGui,
			mSpacingValInTooltipGui));

		mDrawTooltipForNextFrame = true;
	}
}

void GuiButton::setSpriteTexture(const sf::Texture& texture, const sf::IntRect& intRect)
{
	mGuiButtonSprite.setTexture(texture);
	if (intRect != sf::IntRect())
		mGuiButtonSprite.setTextureRect(intRect);

	/*mGuiSprite.setOrigin(mGuiSprite.getTextureRect().width / 2.f,
	mGuiSprite.getTextureRect().height / 2.f);*/
}

void GuiButton::finalizeDrawableGuiList()
{
	pushDrawableObj(mGuiButtonSprite);
	if (mDrawTooltipForNextFrame){
		mDrawTooltipForNextFrame = false;
		pushDrawableObj(mTooltipRectShape);
		pushDrawableObj(mTooltipDrawableText);
	}
}

void GuiButton::handleOnClick(sf::Mouse::Button button,
	const sf::Vector2f& mousePosRelativeToWindow)
{
	if (!mBaseAction.get())
		return;


	sf::Vector2f guiPosRelative = getPosRelativeToWindow();

	sf::FloatRect buttonGlobalBound = mGuiButtonSprite.getGlobalBounds();
	buttonGlobalBound.left += guiPosRelative.x;
	buttonGlobalBound.top += guiPosRelative.y;

	if (button == sf::Mouse::Left && buttonGlobalBound.
		contains(mousePosRelativeToWindow))
	{
		mBaseAction->execute();
	}

}


void GuiButton::setTooltipText(const std::string& text)
{
	std::string structureText = text;

	int numOfNextLine =  (structureText.length() * mCharacterSize) / mLimitSizePerLine;

	int perNextLineIndex = mLimitSizePerLine / mCharacterSize;

	for (int i = 1; i <= numOfNextLine; i++)
		structureText.insert((i * perNextLineIndex) - 1, "\n");
	mTooltipDrawableText.setString(structureText);

	mTooltipRectShape.setSize(sf::Vector2f(240.f + mSpacingValInTooltipGui, 
		(40.f * (numOfNextLine + 1)) + mSpacingValInTooltipGui));
	
}

void GuiButton::handleMouseHover(const sf::Vector2f& mousePosRelativeToWindow)
{
}
#pragma once
#include "BaseGuiComponent.h"
#include "GUIAction/BaseAction.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class GuiButton : public BaseGuiComponent{
public:
	
public:
	GuiButton(BaseAction::Ptr baseAction);
	virtual ~GuiButton();


	void update(sf::RenderWindow& renderWindow, sf::Time dt) override;
	void setSpriteTexture(const sf::Texture& texture, const sf::IntRect& intRect = sf::IntRect());
	void setTooltipText(const std::string& text);

	
private:
	virtual void finalizeDrawableGuiList();

	virtual void handleOnClick(sf::Mouse::Button button,
		const sf::Vector2f& mousePosRelativeToWindow);

	void handleMouseHover(const sf::Vector2f& mousePosRelativeToWindow) override;
private:
	sf::Sprite mGuiButtonSprite;
	BaseAction::Ptr mBaseAction;

	sf::Text mTooltipDrawableText;
	int mCharacterSize;

	sf::RectangleShape mTooltipRectShape;
	sf::Font mFont;
	int mLimitSizePerLine;

	bool mDrawTooltipForNextFrame;

	float mSpacingValInTooltipGui;
};


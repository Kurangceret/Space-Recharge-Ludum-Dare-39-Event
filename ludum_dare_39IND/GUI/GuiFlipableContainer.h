#pragma once
#include "BaseGuiComponent.h"
#include <SFML/System/Vector2.hpp>

class GuiFlipableContainer :	public BaseGuiComponent{
public:
	GuiFlipableContainer();
	virtual ~GuiFlipableContainer();
	
	//remember, any gui component thats being pushed here will adhere
	//to this container relative position
	void pushGuiToBeContained(BaseGuiComponent::Ptr& baseGuiComp);

	void update(sf::RenderWindow& renderWindow, sf::Time dt) override;

	//setting a single arrow will be applicable to the other one, we just need to flip it
	void setRightArrowTexture(const sf::Texture& texture, const sf::IntRect& intRect = sf::IntRect());

	void cleanContainer();
private:
	void finalizeDrawableGuiList() override;

	void handleOnClick(sf::Mouse::Button button,
		const sf::Vector2f& mousePosRelativeToWindow) override;

	void nextPage();
	void prevPage();

private:
	std::vector<BaseGuiComponent::Ptr> mGuiToBeContainedList;

	sf::Vector2f mSpacingPerGui;
	sf::Vector2f mSpacingFromArrow;

	int mNumGuiPerPage;
	//start from 0(think like array~)
	int mCurrentPage;
	
	sf::Sprite mLeftArrowSprite;
	sf::Sprite mRightArrowSprite;

	int mLimitPage;
};


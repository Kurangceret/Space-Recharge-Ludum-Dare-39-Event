#include "GuiFlipableContainer.h"


GuiFlipableContainer::GuiFlipableContainer()
:mNumGuiPerPage(4),
mCurrentPage(0),
mSpacingPerGui(48.f, 0.f),
mSpacingFromArrow(32.f, 0.f),
mLimitPage(0)
{
}


GuiFlipableContainer::~GuiFlipableContainer()
{
}


void GuiFlipableContainer::pushGuiToBeContained(BaseGuiComponent::Ptr& baseGuiComp)
{
	mGuiToBeContainedList.push_back(baseGuiComp);
}

void GuiFlipableContainer::update(sf::RenderWindow& renderWindow, sf::Time dt)
{
	int startIndexToBeShow = mNumGuiPerPage * mCurrentPage;
	int endIndexToBeShow = startIndexToBeShow + (mNumGuiPerPage - 1);

	//int curIndexTobeShow = 0;
	sf::Vector2f mStartRelativePos = getPosRelativeToWindow() + mSpacingFromArrow;

	int countAllIndex = 0;
	for (auto iter = mGuiToBeContainedList.begin(); iter != mGuiToBeContainedList.end();)
	{
		auto baseGui = iter->get();

		if (baseGui->isRemoved()){
			iter = mGuiToBeContainedList.erase(iter);
			continue;
		}

		if (countAllIndex >= startIndexToBeShow && countAllIndex <= endIndexToBeShow)
		{
			baseGui->setIsHidden(false);

			int curIndexDiff = countAllIndex - startIndexToBeShow;
			baseGui->setPositionOnWindow(mStartRelativePos + (mSpacingPerGui 
				* static_cast<float>(curIndexDiff)));
		}
		else{
			baseGui->setIsHidden(true);
		}
				
		iter++;
		countAllIndex++;
	}
	mLimitPage = 0;
	if (mGuiToBeContainedList.size() > 0)
		mLimitPage = static_cast<int>(std::floor(static_cast<float>(
			mGuiToBeContainedList.size()) / static_cast<float>(mNumGuiPerPage)));
}

//setting a single arrow will be applicable to the other one, we just need to flip it
void GuiFlipableContainer::setRightArrowTexture(const sf::Texture& texture,
	const sf::IntRect& intRect)
{
	mRightArrowSprite.setTexture(texture);
	sf::IntRect finalRect = intRect;

	if (finalRect == sf::IntRect())
		finalRect = mRightArrowSprite.getTextureRect();

	mRightArrowSprite.setTextureRect(finalRect);

	finalRect.left += finalRect.width;
	finalRect.width *= -1.f;

	mLeftArrowSprite.setTexture(texture);
	mLeftArrowSprite.setTextureRect(finalRect);

	mLeftArrowSprite.setPosition(0.f, 12.f);

	sf::Vector2f rightEnd((mSpacingFromArrow) + 
		(mSpacingPerGui * static_cast<float>(mNumGuiPerPage)));
	mRightArrowSprite.setPosition(rightEnd + sf::Vector2f(0.f, 12.f));
}


void GuiFlipableContainer::finalizeDrawableGuiList()
{
	pushDrawableObj(mLeftArrowSprite);
	pushDrawableObj(mRightArrowSprite);
}

void GuiFlipableContainer::handleOnClick(sf::Mouse::Button button,
	const sf::Vector2f& mousePosRelativeToWindow)
{
	sf::FloatRect leftBound = mLeftArrowSprite.getGlobalBounds();
	sf::FloatRect rightBound = mRightArrowSprite.getGlobalBounds();
	sf::Vector2f posRelativeToWindow = getPosRelativeToWindow();
	leftBound.left += posRelativeToWindow.x;
	leftBound.top += posRelativeToWindow.y;
	rightBound.left += posRelativeToWindow.x;
	rightBound.top += posRelativeToWindow.y;

	if (leftBound.contains(mousePosRelativeToWindow))
		prevPage();
	else if (rightBound.contains(mousePosRelativeToWindow))
		nextPage();
}

void GuiFlipableContainer::nextPage()
{
	mCurrentPage = std::min(mCurrentPage + 1, mLimitPage);
}

void GuiFlipableContainer::prevPage()
{
	mCurrentPage = std::max(mCurrentPage - 1, 0);
}

void GuiFlipableContainer::cleanContainer()
{
	for (auto& ptr : mGuiToBeContainedList)
		ptr->setIsRemoved(true);

	mGuiToBeContainedList.clear();
}
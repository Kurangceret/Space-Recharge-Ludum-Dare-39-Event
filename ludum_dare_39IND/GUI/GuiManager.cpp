#include "GuiManager.h"


GuiManager::GuiManager()
{
}


GuiManager::~GuiManager()
{
}


void GuiManager::update(sf::RenderWindow& renderWindow, sf::Time dt)
{
	while (!mToBeAddedGuiQueue.empty()){
		mGuiList.push_back(mToBeAddedGuiQueue.front());
		mToBeAddedGuiQueue.pop();
	}

	for (auto iter = mGuiList.begin(); iter != mGuiList.end();){
		if (iter->get()->isRemoved()){
			iter = mGuiList.erase(iter);
			continue;
		}
		iter->get()->update(renderWindow, dt);
		iter++;
	}
}

void GuiManager::handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event)
{
	sf::Vector2f mousePosRelativeToWindow(sf::Mouse::getPosition(renderWindow));
	for (auto& ptr : mGuiList)
		ptr->handleMouseEvent(event, mousePosRelativeToWindow);
}

void GuiManager::addGuiComponent(BaseGuiComponent::Ptr& ptr)
{
	mToBeAddedGuiQueue.push(ptr);
	//mGuiList.push_back(ptr);
}


void GuiManager::drawGuiComponents(sf::RenderWindow& renderWindow)
{
	sf::View view = renderWindow.getView();
	//window size and position, the top left here indicates the current top left drawable window position
	sf::FloatRect viewPort(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f topLeftPos(viewPort.left, viewPort.top);

	sf::RenderStates states;
	states.transform.translate(topLeftPos);

	for (auto& guiPtr : mGuiList)
		guiPtr->draw(renderWindow, states);
}
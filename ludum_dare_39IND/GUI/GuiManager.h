#pragma once
#include "BaseGuiComponent.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <queue>

class GuiManager{
public:
	GuiManager();
	~GuiManager();

	void update(sf::RenderWindow& renderWindow, sf::Time dt);
	void handleEvent(const sf::RenderWindow& renderWindow, const sf::Event& event);
	void addGuiComponent(BaseGuiComponent::Ptr& ptr);

	void drawGuiComponents(sf::RenderWindow& renderWindow);
private:
	std::vector<BaseGuiComponent::Ptr> mGuiList;
	std::queue<BaseGuiComponent::Ptr> mToBeAddedGuiQueue;
};


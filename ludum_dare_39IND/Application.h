#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include "WindowState/StateStack.h"


class Application{
public:
	Application(luabridge::LuaRef windowSettingTable, lua_State* luaState);
	~Application();


	void run();
	void render();
	void processInputs();
	void update(sf::Time dt);
	void registerStates();
	void bindEngineInterfacesToLuaScript();
	void updateStatistics(sf::Time elapsedTime);
private:
	StateStack mStacks;
	sf::RenderWindow mWindow;
	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t	mStatisticsNumFrames;
};


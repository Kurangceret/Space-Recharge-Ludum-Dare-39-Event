#pragma once
#include "StateStack.h"
#include "LoadingTask.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "GeneralData.h"


struct GuiGameStateData;
class LoadingState : public State{
public:
	LoadingState(StateStack& stack, Context context);
	virtual ~LoadingState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);
	void setCompletion(float percent, std::string currrentProcess);
private:
	sf::Text mLoadingText;
	sf::RectangleShape mProgressBarBackground;
	sf::RectangleShape mProgressBar;

	LoadingTask mLoadingTask;
	std::unique_ptr<GeneralData> mGeneralData;
	std::unique_ptr<GuiGameStateData> mGuiGameStateData;

};


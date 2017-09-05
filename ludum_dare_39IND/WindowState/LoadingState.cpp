#include "LoadingState.h"
#include "../Helper/Utility.h"
#include "GameState.h"
#include "GuiGameState.h"

#include "MenuState.h"
#include "../GUI/GuiButton.h"

#include "../StateEvent/StateEventManager.h"

LoadingState::LoadingState(StateStack& stack, Context context)
:State(stack, context),
mGeneralData(new GeneralData()),
mGuiGameStateData(new GuiGameStateData()),
mLoadingTask(nullptr, nullptr, context.mLuaState, *context.mWindow)
{
	mStateID = States::Loading;

	mLoadingTask.setGeneralData(mGeneralData.get());
	mLoadingTask.setGuiGameStateData(mGuiGameStateData.get());

	sf::RenderWindow& window = *getContext().mWindow;
	sf::Font& font = *context.mFont;
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setCharacterSize(18);
	mLoadingText.setString("Loading Resources");
	Utility::centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f - 100.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));
	mProgressBarBackground.setPosition(10, mLoadingText.getPosition().y + 40);

	mProgressBar.setFillColor(sf::Color(100, 100, 100));
	mProgressBar.setSize(sf::Vector2f(200, 10));
	mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);

	setCompletion(0.f, "Loading Resources");

	mLoadingTask.execute();

		

}


LoadingState::~LoadingState()
{
}


void LoadingState::draw()
{
	//sf::RenderWindow& window = mLoadingTask.getSafeThreadedWindow();
	//window.setView(window.getDefaultView());
	mLoadingTask.getSafeThreadedWindow().draw(mLoadingText);
	mLoadingTask.getSafeThreadedWindow().draw(mProgressBarBackground);
	mLoadingTask.getSafeThreadedWindow().draw(mProgressBar);
}
bool LoadingState::update(sf::Time)
{
	// Update the progress bar from the remote task or finish it
	if (mLoadingTask.isFinished()){
		requestStackPop();

		StateEventManager::ShrPtr stateEventManager(new StateEventManager(
			StateEventDataToBeHandled(mGeneralData.get(), mGuiGameStateData.get())));

		GameState::Ptr gameState(new GameState(
			*getStateStack(), getContext(), std::move(mGeneralData), stateEventManager));

		GuiGameState::Ptr guiGameState(new GuiGameState(
			*getStateStack(), getContext(), std::move(mGuiGameStateData), stateEventManager));

		requestStackPush(std::move(gameState));
		requestStackPush(std::move(guiGameState));
	}
	else if (mLoadingTask.isThreadTerminated()){
		requestStackPop();
	}
	else{
		setCompletion(mLoadingTask.getCompletion(), mLoadingTask.getCurrentText());
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}

void LoadingState::setCompletion(float percent, std::string currentProcess)
{
	if (percent > 1.f) // clamp
		percent = 1.f;

	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x * percent, mProgressBar.getSize().y));
	mLoadingText.setString(currentProcess);
}
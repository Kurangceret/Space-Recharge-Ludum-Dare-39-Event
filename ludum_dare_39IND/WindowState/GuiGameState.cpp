#include "GuiGameState.h"
#include "../StateEvent/StandardStateEvent.h"
#include "../GUI/GuiButton.h"

#include "../Helper/Constant.h"
#include "../Event/EventManager.h"

GuiGameState::GuiGameState(StateStack& stack, Context context, GuiGameStateData::Ptr guiStateData,
	StateEventManager::ShrPtr& stateEventManagerPtr)
:State(stack, context),
mGuiStateData(std::move(guiStateData)),
mRenderWindow(*context.mWindow),
mStateEventManager(stateEventManagerPtr)
{
	mStateID = States::GuiGame;
	mStateEventManager->pushNextStateEvent(std::move(StandardStateEvent::Ptr(
		new StandardStateEvent(mStateEventManager.get()))), nullptr);
	
	//mGuiStateData->mMouseManager->pushNextMouseState(std::move(StandardMouseState::Ptr(new StandardMouseState())));
}


GuiGameState::~GuiGameState()
{
}


void GuiGameState::draw()
{
	//mGuiStateData->mMouseManager->drawCurrentState(mRenderWindow);
	mStateEventManager->drawStateEvent(mRenderWindow, sf::RenderStates(), mStateID);
	mGuiStateData->mGuiManager->drawGuiComponents(mRenderWindow);
}

bool GuiGameState::update(sf::Time dt)
{
	mStateEventManager->updateOnState(mRenderWindow, dt, mStateID);
	//mGuiStateData->mMouseManager->processRealTimeMouseInput(mRenderWindow, dt);
	mGuiStateData->mGuiManager->update(mRenderWindow, dt);
	return true;
}

bool GuiGameState::handleEvent(const sf::Event& event)
{
	/*mGuiStateData->mMouseManager->checkForNextMouseState();
	mGuiStateData->mMouseManager->handleMouseEvent(mRenderWindow, event);*/
	mStateEventManager->checkAnyState(mGuiStateData->mLuaState);
	mStateEventManager->handleStateEvent(mRenderWindow, event, mStateID);
	return true;
}
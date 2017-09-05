#include "StateEventManager.h"
#include "BaseStateEvent.h"
#include <iostream>


StateEventDataToBeHandled::StateEventDataToBeHandled(
	GeneralData* generalData, GuiGameStateData* guiGameStateData)
:mGeneralData(generalData),
mGuiGameStateData(guiGameStateData)
{
}

StateEventManager::StateCommand::StateCommand(StateCommandIdent commandIdent, BaseStateEvent* stateSenderPtr)
: mStateCommandIdent(commandIdent),
mStateSenderPtr(stateSenderPtr)
{}

StateEventManager::StateEventManager(const StateEventDataToBeHandled& dataToBeHandled)
:mStateDataToBeHandled(dataToBeHandled)
{
}


StateEventManager::~StateEventManager()
{
}

void StateEventManager::handleCompleteNewStateCommand(BaseStateEventPtr& statePtr, lua_State* luaState)
{
	if (!statePtr->attemptToEnterStateEvent(luaState))
		return;
	

	for (auto& ptr : mCurStateEventPtrList){
		if (!ptr->isInitialized())
			continue;
		ptr->exitStateEvent();
	}

	statePtr->initializeStateEvent(luaState);

	mCurStateEventPtrList.clear();
	mCurStateEventPtrList.push_back(std::move(statePtr));
}

void StateEventManager::handleAddThisStateCommand(BaseStateEventPtr& statePtr, BaseStateEvent* commandSender, 
	lua_State* luaState)
{
	if (!statePtr->attemptToEnterStateEvent(luaState))
		return;
	

	statePtr->handleCreatorStateEvent(commandSender);
	statePtr->initializeStateEvent(luaState);

	for (auto& ptr : mCurStateEventPtrList)
		ptr->handleNewAssistStateEvent(statePtr.get());
		
	mCurStateEventPtrList.push_back(std::move(statePtr));
}

void StateEventManager::handleDeleteThisStateCommand(BaseStateEventPtr& statePtr)
{
	if (mCurStateEventPtrList.size() <= 1)
		return;

	StateEventsIdentifier::ID stateEventIdent = statePtr->getStateEventIdent();
	auto findIter = std::find_if(mCurStateEventPtrList.begin(), mCurStateEventPtrList.end(), 
		[&](BaseStateEventPtr& ptr) -> bool
	{
		return ptr->getStateEventIdent() == stateEventIdent;
	});

	if (findIter == mCurStateEventPtrList.end())
		return;
	
	auto findBaseStatePtr = findIter->get();

	if (findBaseStatePtr->isInitialized())
		findBaseStatePtr->exitStateEvent();

	for (auto& ptr : mCurStateEventPtrList){
		if (ptr->getStateEventIdent() == findBaseStatePtr->getStateEventIdent())
			continue;
		ptr->handleDeletedAssistStateEvent(findBaseStatePtr);
	}
	statePtr.release();
	mCurStateEventPtrList.erase(findIter);
}

void StateEventManager::checkAnyState(lua_State* luaState)
{
	while (!mQueueCommand.empty()){
		auto& commandRef = mQueueCommand.front();
		switch (commandRef.second.mStateCommandIdent){
		case StateCommandIdent::AddThisState:
			handleAddThisStateCommand(commandRef.first, 
				commandRef.second.mStateSenderPtr, luaState);
			break;
		case StateCommandIdent::CompleteNewState:
			handleCompleteNewStateCommand(commandRef.first, luaState);
			break;
		case StateCommandIdent::DeleteThisState:
			handleDeleteThisStateCommand(commandRef.first);
			break;
		default:
			break;
		}
		mQueueCommand.pop();
	}
}

void StateEventManager::drawStateEvent(sf::RenderTarget& renderTarget, sf::RenderStates states,
	std::size_t stateIdToHandle)
{
	
	if (mCurStateEventPtrList.empty())
		return;

	for (auto& ptrIter : mCurStateEventPtrList){
		switch (stateIdToHandle){
		case States::Game:
			ptrIter->drawOnGameState(renderTarget, states);
			break;
		case States::GuiGame:
			ptrIter->drawOnGuiGameState(renderTarget, states);
			break;
		default:
			break;
		}
	}
}

void StateEventManager::updateOnState(sf::RenderWindow& renderWindow, sf::Time dt,
	std::size_t stateIdToHandle)
{
	if (mCurStateEventPtrList.empty())
		return;

	for (auto& ptrIter : mCurStateEventPtrList){
		switch (stateIdToHandle){
		case States::Game:
			ptrIter->updateOnGameState(mStateDataToBeHandled.mGeneralData,
				renderWindow, dt);
			break;
		case States::GuiGame:
			ptrIter->updateOnGuiGameState(mStateDataToBeHandled.mGuiGameStateData,
				renderWindow, dt);
			break;
		default:
			break;
		}
	}
}

void StateEventManager::handleStateEvent(sf::RenderWindow& renderWindow, const sf::Event& event,
	std::size_t stateIdToHandle)
{
	
	if (mCurStateEventPtrList.empty())
		return;

	for (auto& ptrIter : mCurStateEventPtrList){
		switch (stateIdToHandle){
		case States::Game:
			ptrIter->handleGameStateEvent(mStateDataToBeHandled.mGeneralData,
				renderWindow, event);
			break;
		case States::GuiGame:
			ptrIter->handleGuiGameStateEvent(mStateDataToBeHandled.mGuiGameStateData,
				renderWindow, event);
			break;
		default:
			break;
		}
	}
}

void StateEventManager::pushNextStateEvent(BaseStateEventPtr nextStatePtr,
	BaseStateEvent* commandSender)
{
	mQueueCommand.push(std::make_pair(std::move(nextStatePtr), 
		StateCommand(StateCommandIdent::CompleteNewState, commandSender)));
}

void StateEventManager::pushAssistStateEvent(BaseStateEventPtr baseStateEventPtr,
	BaseStateEvent* commandSender)
{
	mQueueCommand.push(std::make_pair(std::move(baseStateEventPtr),
		StateCommand(StateCommandIdent::AddThisState, commandSender)));
}

void StateEventManager::deleteStateEvent(BaseStateEvent* baseStateEvent,
	BaseStateEvent* commandSender)
{
	BaseStateEventPtr baseStatePtr;
	baseStatePtr.reset(baseStateEvent);
	mQueueCommand.push(std::make_pair(std::move(baseStatePtr),
		StateCommand(StateCommandIdent::DeleteThisState, commandSender)));
}

/*void StateEventManager::pushAssistForCurStateEvent(BaseStateEventPtr assistStatePtr,
	bool beforeCheckNewState)
{
	mNextAssistStateEventPtrList.push_back(std::make_pair(std::move(assistStatePtr), beforeCheckNewState));
	//mCurAssistStateEventPtr = std::move(assistStatePtr);
}*/

/*void StateEventManager::checkForAssistStates(lua_State* luaState, bool beforeCheckNewState)
{
	if (mNextAssistStateEventPtrList.empty())
		return;

	NextAssistStatesContainer::iterator pairIter = mNextAssistStateEventPtrList.begin();
	while(pairIter != mNextAssistStateEventPtrList.end()){
		if (pairIter->second != beforeCheckNewState){
			pairIter++;
			continue;
		}

		if (pairIter->first->attemptToEnterStateEvent(luaState)){
			pairIter->first->initializeStateEvent(luaState);
			mCurStateEventPtrList.push_back(std::move(pairIter->first));
		}
		pairIter = mNextAssistStateEventPtrList.erase(pairIter);
	}
}*/
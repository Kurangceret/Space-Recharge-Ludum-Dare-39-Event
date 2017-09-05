#pragma once
#include <vector>
#include <queue>
#include "../WindowState/WindowStateIdentifier.h"
#include "StateEventsIdentifier.h"
#include <memory>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include <unordered_map>

class Entity;
class BaseStateEvent;
class GeneralData;
struct GuiGameStateData;
typedef std::unique_ptr<BaseStateEvent> BaseStateEventPtr;
struct lua_State;

struct StateEventDataToBeHandled{
	StateEventDataToBeHandled(GeneralData* generalData, GuiGameStateData* guiGameStateData);
	GeneralData* mGeneralData;
	GuiGameStateData* mGuiGameStateData;
};



class StateEventManager{
public:
	enum StateCommandIdent{
		CompleteNewState,
		DeleteThisState,
		AddThisState,
		StateCommandCount
	};
	struct StateCommand{
		StateCommand() = default;
		StateCommand(StateCommandIdent commandIdent, BaseStateEvent* stateSenderPtr);
		StateCommandIdent mStateCommandIdent;
		//StateEventsIdentifier::ID mCommandSender;
		BaseStateEvent* mStateSenderPtr;
	};
public:
	typedef std::unique_ptr<StateEventManager> Ptr;
	typedef std::shared_ptr<StateEventManager> ShrPtr;
	typedef std::vector<std::pair<BaseStateEventPtr, bool>> NextAssistStatesContainer;
	typedef std::queue<std::pair<BaseStateEventPtr, StateCommand>> QueueCommand;
	//typedef std::pair<StateEventsIdentifier::ID, BaseStateEventPtr> StatePair;
	typedef std::vector<BaseStateEventPtr> StatePtrContainer;

public:
	StateEventManager(const StateEventDataToBeHandled& dataToBeHandled);
	~StateEventManager();

	void checkAnyState(lua_State* luaState);

	void drawStateEvent(sf::RenderTarget& renderTarget, sf::RenderStates states, 
		std::size_t stateIdToHandle);
	void updateOnState(sf::RenderWindow& renderWindow, sf::Time dt,
		std::size_t stateIdToHandle);
	void handleStateEvent(sf::RenderWindow& renderWindow, const sf::Event& event,
		std::size_t stateIdToHandle);

	void pushNextStateEvent(BaseStateEventPtr nextStatePtr, 
		BaseStateEvent* commandSender);
	void pushAssistStateEvent(BaseStateEventPtr baseStateEventPtr,
		BaseStateEvent* commandSender);
	void deleteStateEvent(BaseStateEvent* baseStateEvent,
		BaseStateEvent* commandSender);
	//void pushAssistForCurStateEvent(BaseStateEventPtr assistStatePtr, 
		//bool beforeCheckNewState = true);

private:
	void handleCompleteNewStateCommand(BaseStateEventPtr& statePtr, lua_State* luaState);
	void handleAddThisStateCommand(BaseStateEventPtr& statePtr, BaseStateEvent* commandSender,
		lua_State* luaState);
	void handleDeleteThisStateCommand(BaseStateEventPtr& statePtr);
	//void checkForAssistStates(lua_State* luaState, bool beforeCheckNewState = true);
private:
	//std::vector<BaseStateEventPtr> mCurStateEventPtrList;
	//NextAssistStatesContainer mNextAssistStateEventPtrList;

	QueueCommand mQueueCommand;
	StatePtrContainer mCurStateEventPtrList;

	//BaseStateEventPtr mCurStateEventPtr;
	//BaseStateEventPtr mCurAssistStateEventPtr;
	//BaseStateEventPtr mNextStateEventPtr;

	StateEventDataToBeHandled mStateDataToBeHandled;
};


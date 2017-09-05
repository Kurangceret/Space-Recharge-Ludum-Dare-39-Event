#pragma once
#include "../WindowState/WindowStateIdentifier.h"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "StateEventManager.h"
#include <functional>
#include "StateEventsIdentifier.h"

class StateEventManager;
class GeneralData;
struct GuiGameStateData;
struct lua_State;



class BaseStateEvent{
public:
	typedef std::unique_ptr<BaseStateEvent> Ptr;

	friend StateEventManager;
	typedef std::function<void(GeneralData*, sf::RenderWindow&, 
		sf::Time)> GameStateUpdateFunc;
	typedef std::function<void(GeneralData*, sf::RenderWindow&, 
		const sf::Event&)> GameStateEventFunc;

	static GameStateUpdateFunc StandardPlayerMovementFunc;
	static GameStateUpdateFunc StandardPlayerBodyStatusFunc;
	static GameStateUpdateFunc CameraOnPlayerFunc;
	static GameStateUpdateFunc ArrowKeyPlayerMovementFunc;

	static GameStateUpdateFunc JumpStateUpdateFunc;
	static GameStateEventFunc JumpHandleKeyEvent;

	typedef std::function<void(GeneralData*, sf::RenderWindow&,
		float, float, float, sf::Time)> IncrementalMovementFunc;

	static IncrementalMovementFunc IncrementMovementXPlayerFunc;
	static IncrementalMovementFunc IncrementMovementYPlayerFunc;
public:
	virtual ~BaseStateEvent();

	//if this function return false, the queed state event will be rejected
	//here is also the part we check whether a state condition has been met
	//to be entered
	virtual bool attemptToEnterStateEvent(lua_State* luaState) const;

	//this part will be called after the newStateEvent is ackwnoeldged
	//to be valid

	virtual void exitStateEvent();
	//this part will be caleld after prev state has been exited
	virtual void initializeStateEvent(lua_State* luaState);
	
	bool isInitialized() const;
	StateEventsIdentifier::ID getStateEventIdent() const;
protected:
	BaseStateEvent(StateEventManager* stateEventManager, 
		StateEventsIdentifier::ID stateEventIdent = StateEventsIdentifier::Base);

	virtual void handleCreatorStateEvent(BaseStateEvent* thisStateEventCreator);
	virtual void handleNewAssistStateEvent(BaseStateEvent* baseStateEvent);
	virtual void handleDeletedAssistStateEvent(BaseStateEvent* baseStateEvent);

	virtual void drawOnGameState(sf::RenderTarget& renderTarget, 
		sf::RenderStates states);

	virtual void updateOnGameState(GeneralData* generalData,
		sf::RenderWindow& renderWindow, 
		sf::Time dt);
	virtual void handleGameStateEvent(GeneralData* generalData, 
		sf::RenderWindow& renderWindow,
		const sf::Event& event);


	virtual void drawOnGuiGameState(sf::RenderTarget& renderTarget,
		sf::RenderStates states);

	virtual void updateOnGuiGameState(GuiGameStateData* guiGameStateData,
		sf::RenderWindow& renderWindow, 
		sf::Time dt);
	virtual void handleGuiGameStateEvent(GuiGameStateData* guiGameStateData, 
		sf::RenderWindow& renderWindow,
		const sf::Event& event);
	

	void removeThisStateFromManager();

	template<typename E, typename ... Args>
	void pushNextStateEvent(Args&& ... args);
	
	template<typename E, typename ... Args>
	void pushAssistStateEvent(Args&& ... args);
	/*template<typename E, typename ... Args>
	void pushNextAssistStateEvent(bool beforeCheckNewState, Args&& ... args);*/

private:
	StateEventManager* mStateEventManager;
	bool mIsInitialized;
	StateEventsIdentifier::ID mStateEventIdent;
};

template<typename E, typename ... Args>
void BaseStateEvent::pushNextStateEvent(Args&& ... args)
{
	BaseStateEvent::Ptr baseState(new E(mStateEventManager, std::forward<Args>(args) ...));
	mStateEventManager->pushNextStateEvent(std::move(baseState), this);
}

/*template<typename E, typename ... Args>
void BaseStateEvent::pushNextAssistStateEvent(bool beforeCheckNewState, Args&& ... args)
{
	BaseStateEvent::Ptr baseState(new E(mStateEventManager, std::forward<Args>(args) ...));
	mStateEventManager->pushAssistForCurStateEvent(std::move(baseState), beforeCheckNewState);
}*/

template<typename E, typename ... Args>
void BaseStateEvent::pushAssistStateEvent(Args&& ... args)
{
	BaseStateEvent::Ptr baseState(new E(mStateEventManager, std::forward<Args>(args) ...));
	mStateEventManager->pushAssistStateEvent(std::move(baseState), this);
}
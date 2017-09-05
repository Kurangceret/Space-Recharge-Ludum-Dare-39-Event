#pragma once
#include "State.h"
#include "WindowStateIdentifier.h"
#include <functional>
#include <vector>
#include <map>
#include <cassert>

struct lua_State;

class StateStack{
public:
	enum Action{
		Push,
		Pop,
		Clear
	};
public:
	explicit StateStack(State::Context context);
	~StateStack();
	


	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event&);

	void pushState(State::Ptr statePtr);
	void popState();
	void clearStates();

	State::Context& getCurrentStateContext();
	lua_State* getLuaState();

	bool isEmpty() const { return mStack.empty(); }

private:
	//State::Ptr createState(States::ID stateID);
	void applyPendingChanges();

private:
	struct PendingChange{
		explicit PendingChange(Action action, States::ID stateID, 
			State::Ptr state = State::Ptr())
		:action(action), stateID(stateID),
		mNewState(std::move(state))
		{}
		Action	action;
		States::ID	stateID; // only applicable if action is delete
		State::Ptr mNewState; // only applicable if action is push
		PendingChange(StateStack::PendingChange &);
	};

private:
	std::vector<State::Ptr> mStack;	//keeps all the State 
	std::vector<PendingChange> mPendingList;	//keeps all the change/action to be done
	State::Context mContext;
	//mFactories hold all the states represented by States::ID and
	//the value is a function to be called to load the states
	//std::map<States::ID, std::function<State::Ptr()>> mFactories;
};


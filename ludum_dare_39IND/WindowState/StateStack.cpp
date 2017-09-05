#include "StateStack.h"


StateStack::StateStack(State::Context context)
:mContext(context)
{
	
}

StateStack::PendingChange::PendingChange(StateStack::PendingChange & pendingChange)
:action(pendingChange.action),
stateID(pendingChange.stateID),
mNewState(pendingChange.mNewState.release())
{

}


/*State::Ptr StateStack::createState(States::ID id)
{
	auto found = mFactories.find(id);
	assert(found != mFactories.end());

	return found->second();
}*/

void StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++){
		if (!(*itr)->update(dt))
			break;
	}
	applyPendingChanges();
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++){
		if (!(*itr)->handleEvent(event))
			break;
	}
	applyPendingChanges();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange& change : mPendingList){
		switch (change.action){
		case Push:
			mStack.push_back(std::move(change.mNewState));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

void StateStack::draw()
{
	for (State::Ptr& ptr : mStack)
		ptr->draw();
}

void StateStack::pushState(State::Ptr statePtr)
{
	mPendingList.push_back(PendingChange(Push, States::None, std::move(statePtr)));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop, States::None));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear, States::None));
}

StateStack::~StateStack()
{
}

lua_State* StateStack::getLuaState()
{
	return mContext.mLuaState;
}


State::Context& StateStack::getCurrentStateContext()
{
	return mContext;
}
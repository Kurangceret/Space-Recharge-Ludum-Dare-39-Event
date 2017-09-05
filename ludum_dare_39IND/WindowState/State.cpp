#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack, Context context)
:mStateStack(&stack), 
mContext(context),
mStateID(States::None)
{
}


State::~State()
{
}

StateStack* State::getStateStack() const
{
	return mStateStack;
}

void State::requestStackPush(State::Ptr statePtr)
{
	mStateStack->pushState(std::move(statePtr));
}

void State::requestStackPop()
{
	mStateStack->popState();
}

void State::requestStackClear()
{
	mStateStack->clearStates();
}


State::Context State::getContext()  const
{
	return mContext;
}
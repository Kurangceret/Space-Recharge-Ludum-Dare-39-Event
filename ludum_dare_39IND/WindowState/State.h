#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include "GeneralData.h"
#include "WindowStateIdentifier.h"

struct lua_State;
class StateStack;


class State{
public:
	struct Context{
		Context(sf::RenderWindow& window, sf::Font& font, lua_State* luaState/*, GeneralData& generalData*/)
		:mWindow(&window), mFont(&font), mLuaState(luaState)/*, generalData(&generalData)*/{}

		sf::RenderWindow* mWindow;
		sf::Font* mFont;
		lua_State* mLuaState;
		//GeneralData* generalData;
	};

	typedef std::unique_ptr<State> Ptr;
public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event&) = 0;

protected:
	void requestStackClear();
	void requestStackPop();
	void requestStackPush(State::Ptr statePtr);

	Context getContext()  const;
	StateStack* getStateStack() const;

	std::size_t mStateID;
private:
	StateStack *mStateStack;
	Context mContext;
	
};


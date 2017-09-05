#pragma once
#include "StateStack.h"
#include "../GUI/GuiManager.h"
#include "../StateEvent/StateEventManager.h"



struct lua_State;

struct GuiGameStateData{
	typedef std::unique_ptr<GuiGameStateData> Ptr;
	std::unique_ptr<GuiManager> mGuiManager;
	lua_State* mLuaState;
};

class GuiGameState :public State{
public:
	GuiGameState(StateStack& stack, Context context, GuiGameStateData::Ptr guiStateData,
		StateEventManager::ShrPtr& stateEventManagerPtr);

	virtual ~GuiGameState();
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

private:
	GuiGameStateData::Ptr mGuiStateData;
	sf::RenderWindow& mRenderWindow;
	StateEventManager::ShrPtr mStateEventManager;
	/*std::unique_ptr<GuiManager> mGuiManager;
	std::unique_ptr<MouseStateManager> mMouseManager;*/
};


#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Thor\Particles\ParticleSystem.hpp>
#include "../System/Camera.h"
#include <SFML/Graphics/Texture.hpp>

#include "../CustomBox2D/SfDebugB2Draw.h"
#include "../CustomBox2D/EngineContactListener.h"

class PrintEvent;

class GeneralData;
struct lua_State;
class StateEventManager;

class Game{
public:
	Game(GeneralData* generalData, sf::RenderWindow& window, lua_State* luaState,
		StateEventManager* stateEventManager);
	~Game();

	void draw();
	void update(sf::Time dt);
	void handleEvent(const sf::Event&);
private:
	void bindPrintEvent(const PrintEvent* printEvent);

	void processCameraInput(sf::Time dt);
private:
	sf::RenderWindow& mWindow;
	GeneralData* mGeneralData;
	lua_State* mLuaState;
	//WaveEntitySpawner mWaveEntitySpawner;
	//FourTree mFourTree;
	StateEventManager* mStateEventManager;
	SfDebugB2Draw mSfDebugB2Draw;
	EngineContactListener mEngineContactListener;

	bool mDoEventCheck;

	sf::Sprite mBGSprite;

	thor::ParticleSystem m_particle_system;
	sf::Texture m_particle_texture;
	//Camera mCamera;
};


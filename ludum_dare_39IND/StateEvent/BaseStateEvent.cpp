#include "BaseStateEvent.h"
#include "../System/AllSystemsHeader.h"
#include "../WindowState/GeneralData.h"
#include "../WindowState/GuiGameState.h"


#include "../Component/Body2dComponent.h"
#include "../Component/PlayerComponent.h"
#include "../Component/JumpComponent.h"
#include "../Entity/Entity.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"


BaseStateEvent::GameStateUpdateFunc BaseStateEvent::ArrowKeyPlayerMovementFunc
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, sf::Time dt)
{
	sf::Vector2f finalKeyV;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		finalKeyV.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		finalKeyV.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		finalKeyV.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		finalKeyV.x = 1.f;
	Body2dComponent* playerBody = generalData->mPlayerEntity->getComp<Body2dComponent>();
	playerBody->setVelocityX(finalKeyV.x);
	//generalData->mPlayerEntity->getComp<VelocityComponent>()->setVelocity(finalKeyV);
};
BaseStateEvent::GameStateUpdateFunc BaseStateEvent::StandardPlayerMovementFunc 
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, sf::Time dt)
{
	sf::Vector2f finalKeyV;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		finalKeyV.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		finalKeyV.y = 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		finalKeyV.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		finalKeyV.x = 1.f;
	
	generalData->mPlayerEntity->getComp<Body2dComponent>()->setVelocity(finalKeyV);
};



BaseStateEvent::GameStateUpdateFunc BaseStateEvent::StandardPlayerBodyStatusFunc
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, sf::Time dt)
{
	Body2dComponent* bodyComp = generalData->mPlayerEntity->getComp<Body2dComponent>();
	bodyComp->setBodyAwake(true);
};

BaseStateEvent::GameStateUpdateFunc BaseStateEvent::CameraOnPlayerFunc
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, sf::Time dt)
{
	Body2dComponent* bodyComp = generalData->mPlayerEntity->getComp<Body2dComponent>();
	sf::Vector2f playerPos = bodyComp->getSfWorldPosition();
	PlayerComponent* playerComp = generalData->mPlayerEntity->getComp<PlayerComponent>();

	/*if (playerComp->getCurGravityDir().y == 0.f){
		float diff = bodyComp->getFacingDirY() > 0.f ? 100.0 : -100.0;
		playerPos.y += diff;
	}*/
	sf::View curView = renderWindow.getDefaultView();

	generalData->mCamera->goTo(playerPos.x, playerPos.y);
};

BaseStateEvent::IncrementalMovementFunc BaseStateEvent::IncrementMovementXPlayerFunc 
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, 
	float impulseIncrease, float impulseReduce, float maxImpulse, sf::Time dt)
{
	Body2dComponent* bodyComp = generalData->mPlayerEntity->getComp<Body2dComponent>();

	sf::Vector2f finalKeyV;
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		finalKeyV.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		finalKeyV.x = 1.f;

	float frameImpulseInc = impulseIncrease * dt.asSeconds();
	float frameImpulseRed = impulseReduce * dt.asSeconds();

	sf::Vector2f finalImpulse;
	sf::Vector2f curImpulse = bodyComp->getSfLinearVelocity();

	int identifierNum = curImpulse.x > 0.f ? -1 : 1;

	if (finalKeyV.x == 0.f){
		if (curImpulse.x == 0.f) return;
		
		bodyComp->moveVeloPixelX(frameImpulseRed * identifierNum, false);
		//bodyComp->applyLinearPixelImpulseToCenter(sf::Vector2f(frameImpulseRed * identifierNum, 0.f));
		int nowIdentiferNum = bodyComp->getSfLinearVelocity().x > 0.f ? -1 : 1;

		if (nowIdentiferNum != identifierNum)
			bodyComp->setVelocityX(0.f);
		return;
	}
	
	//if (std::abs(curImpulse.x) == std::abs(maxImpulse.x))
		//return;
	
	bodyComp->moveVeloPixelX(frameImpulseInc * finalKeyV.x, false);	
	//bodyComp->applyLinearPixelImpulseToCenter(sf::Vector2f(frameImpulseInc * finalKeyV.x, 0.f));

	float absMaxImpulse = std::abs(maxImpulse);
	if (std::abs(bodyComp->getSfLinearVelocity().x) > absMaxImpulse)
		bodyComp->setVelocityX(absMaxImpulse * finalKeyV.x, 0);
};

BaseStateEvent::IncrementalMovementFunc BaseStateEvent::IncrementMovementYPlayerFunc
= [](GeneralData* generalData, sf::RenderWindow& renderWindow,
float impulseIncrease, float impulseReduce, float maxImpulse, sf::Time dt)
{
	Body2dComponent* bodyComp = generalData->mPlayerEntity->getComp<Body2dComponent>();

	sf::Vector2f finalKeyV;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		finalKeyV.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		finalKeyV.y = 1.f;

	float frameImpulseInc = impulseIncrease * dt.asSeconds();
	float frameImpulseRed = impulseReduce * dt.asSeconds();

	sf::Vector2f finalImpulse;
	sf::Vector2f curImpulse = bodyComp->getSfLinearVelocity();

	int identifierNum = curImpulse.y > 0.f ? -1 : 1;

	if (finalKeyV.y == 0.f){
		if (curImpulse.y == 0.f) return;

		bodyComp->moveVeloPixelY(frameImpulseRed * identifierNum, false);
		//bodyComp->applyLinearPixelImpulseToCenter(sf::Vector2f(frameImpulseRed * identifierNum, 0.f));
		int nowIdentiferNum = bodyComp->getSfLinearVelocity().y > 0.f ? -1 : 1;

		if (nowIdentiferNum != identifierNum)
			bodyComp->setVelocityY(0.f);
		return;
	}

	//if (std::abs(curImpulse.x) == std::abs(maxImpulse.x))
	//return;

	bodyComp->moveVeloPixelY(frameImpulseInc * finalKeyV.y, false);
	//bodyComp->applyLinearPixelImpulseToCenter(sf::Vector2f(frameImpulseInc * finalKeyV.x, 0.f));
	float absMaxImpulse = std::abs(maxImpulse);
	if (std::abs(bodyComp->getSfLinearVelocity().y) > absMaxImpulse)
		bodyComp->setVelocityY(absMaxImpulse  * finalKeyV.y, 0);
};



BaseStateEvent::GameStateUpdateFunc BaseStateEvent::JumpStateUpdateFunc
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, sf::Time dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		generalData->mPlayerEntity->getComp<JumpComponent>()->prolongJump();
};


BaseStateEvent::GameStateEventFunc BaseStateEvent::JumpHandleKeyEvent
	= [](GeneralData* generalData, sf::RenderWindow& renderWindow, const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
		JumpComponent* jumpComp = generalData->mPlayerEntity->getComp<JumpComponent>();
		jumpComp->startJumpImpulse();
	}
};


BaseStateEvent::BaseStateEvent(StateEventManager* stateEventManager, 
	StateEventsIdentifier::ID stateEventIdent)
:mStateEventManager(stateEventManager),
mIsInitialized(false),
mStateEventIdent(stateEventIdent)
{
}


BaseStateEvent::~BaseStateEvent()
{
}

StateEventsIdentifier::ID BaseStateEvent::getStateEventIdent() const
{
	return mStateEventIdent;
}

bool BaseStateEvent::attemptToEnterStateEvent(lua_State* luaState) const
{
	return true;
}

void BaseStateEvent::initializeStateEvent(lua_State* luaState)
{
	mIsInitialized = true;
}

bool BaseStateEvent::isInitialized() const
{
	return mIsInitialized;
}

void BaseStateEvent::exitStateEvent()
{
}


void BaseStateEvent::drawOnGameState(sf::RenderTarget& renderTarget,
	sf::RenderStates states)
{

}

void BaseStateEvent::updateOnGameState(GeneralData* generalData,
	sf::RenderWindow& renderWindow,
	sf::Time dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
		Body2dComponent* bodyComp = generalData->mPlayerEntity->getComp<Body2dComponent>();
		sf::Vector2f mouseWorldPos(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow)));
		bodyComp->setPosition(mouseWorldPos);
	}
	/*Entity* playerEntity = generalData->mPlayerEntity;
	//playerEntity->getComp<PlayerComponent>()->setIsInteractingThisFrame(false);
	Body2dComponent* bodyComp = playerEntity->getComp<Body2dComponent>();
	sf::Vector2f playerPos = bodyComp->getSfPosition();
	generalData->mCamera->goTo(playerPos.x, playerPos.y);*/
}

void BaseStateEvent::handleGameStateEvent(GeneralData* generalData,
	sf::RenderWindow& renderWindow,
	const sf::Event& event)
{
	SystemManager* systemManager = generalData->getSystemManager();
		
	//systemManager->handleEvent<PlayerSystem>(renderWindow, event);
	//systemManager->handleEvent<ClickEntitySystem>(renderWindow, event);
}

void BaseStateEvent::drawOnGuiGameState(sf::RenderTarget& renderTarget,
	sf::RenderStates states)
{
	
}

void BaseStateEvent::updateOnGuiGameState(GuiGameStateData* guiStateData,
	sf::RenderWindow& renderWindow,
	sf::Time dt)
{
	guiStateData->mGuiManager->update(renderWindow, dt);
	//guiStateData->mMouseManager->processRealTimeMouseInput(renderWindow, dt);
}

void BaseStateEvent::handleGuiGameStateEvent(GuiGameStateData* guiStateData,
	sf::RenderWindow& renderWindow,
	const sf::Event& event)
{
	
	guiStateData->mGuiManager->handleEvent(renderWindow, event);
	/*guiStateData->mMouseManager->checkForNextMouseState();
	guiStateData->mMouseManager->handleMouseEvent(renderWindow, event);*/
}


void BaseStateEvent::handleNewAssistStateEvent(BaseStateEvent* baseStateEvent)
{

}

void BaseStateEvent::handleDeletedAssistStateEvent(BaseStateEvent* baseStateEvent)
{

}

void BaseStateEvent::handleCreatorStateEvent(BaseStateEvent* thisStateEventCreator)
{

}

void BaseStateEvent::removeThisStateFromManager()
{
	mStateEventManager->deleteStateEvent(this, this);
}
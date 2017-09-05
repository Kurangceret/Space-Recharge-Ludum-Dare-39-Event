#include "JumpSystem.h"
#include "../Helper/Utility.h"
#include "../Entity/Entity.h"
#include "../Component/JumpComponent.h"
#include "../Component/Body2dComponent.h"
#include "../Component/PlayerComponent.h"

JumpSystem::JumpSystem()
{
	pushRequiredComponent(ComponentIdentifier::JumpComponent);
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);
}


JumpSystem::~JumpSystem()
{
}


void JumpSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	JumpComponent* jumpComp = entity->getComp<JumpComponent>();
	if (!jumpComp->isJumping())
		return;

	if (!jumpComp->mWillProlongJump){
		//jumpComp->mCurIncJumpPos = 0.f;
		jumpComp->mCurTimeJumpVeloHold = sf::Time::Zero;
		return;
	}
	if (jumpComp->mCurTimeJumpVeloHold.asSeconds() <= 0.f)
		return;

	Body2dComponent* body2dComp = entity->getComp<Body2dComponent>();
	sf::Vector2f curEntityVelo = body2dComp->getSfLinearVelocity();


	jumpComp->mCurTimeJumpVeloHold -= dt;


	if (std::abs(curEntityVelo.y) < jumpComp->mJumpVeloHoldVal)
		body2dComp->setVelocityY(-jumpComp->mJumpVeloHoldVal, false);
	
	jumpComp->mWillProlongJump = false;

	/*float incJumpPosFrame = jumpComp->mIncJumpPosPerSec * dt.asSeconds();

	body2dComp->move(sf::Vector2f(0.f, -incJumpPosFrame));
	
	jumpComp->mCurIncJumpPos -= incJumpPosFrame;*/
	
}
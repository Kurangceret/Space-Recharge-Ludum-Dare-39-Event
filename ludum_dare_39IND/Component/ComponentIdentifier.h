#ifndef COMPONENTIDENTIFIER_H
#define COMPONENTIDENTIFIER_H


namespace ComponentIdentifier{
	enum ID{
		EmptyComponent = 0,
		AutomaticPathComponent,
		PlayerComponent,
		SpriteComponent,
		HealthComponent,
		EntityParentComponent,
						
		CategoryComponent,
		AnimationComponent,
		ScriptUpdateComponent,
		VertexNodeComponent,
		
		TextDisplayComponent,
		
		DestroyableComponent,
		TimerComponent,
		EntityChildrenComponent,
				
		Body2dComponent,
		
		ObstacleComponent,
		AutoBody2dMovementComponent,
		JumpComponent,
		RealTimeInputComponent,
		
		
		ScriptAIComponent,
		InstanceComponent,

		ParticleEmtitterComponent,

		SpaceShipComponent,
		PlanetSpawnComponent,
		ComponentCounts
	};
}

#endif
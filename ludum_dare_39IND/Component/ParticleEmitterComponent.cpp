#include "ParticleEmitterComponent.h"
#include "ComponentIdentifier.h"


ParticleEmitterComponent::ParticleEmitterComponent(Entity* entity)
:Component(entity, ComponentIdentifier::ParticleEmtitterComponent)
{
}


ParticleEmitterComponent::~ParticleEmitterComponent()
{
}





void ParticleEmitterComponent::pause_all_emitter()
{
	for (auto& emitter : m_emitters_list)
	{
		emitter.m_pause_emitter = true;
	}
}

void ParticleEmitterComponent::continue_all_emitter()
{
	for (auto& emitter : m_emitters_list)
	{
		emitter.m_pause_emitter = false;
	}
}

void ParticleEmitterComponent::set_all_emitter_direction(const sf::Vector2f& new_dir)
{
	for (auto& emitter : m_emitters_list)
	{
		emitter.m_emitter.setParticleVelocity(thor::Distributions::deflect(
			new_dir * emitter.m_particle_magnitude, emitter.m_dir_angle_deviation));
	}
}
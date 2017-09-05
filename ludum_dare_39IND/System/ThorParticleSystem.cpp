#include "ThorParticleSystem.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Component/ComponentIdentifier.h"
#include "../Component/ParticleEmitterComponent.h"
#include "../Helper/Constant.h"

ThorParticleSystem::ThorParticleSystem(TextureStringManager& textures_str_manager)
:m_textures_str_manager(textures_str_manager)
{
	pushRequiredComponent(ComponentIdentifier::ParticleEmtitterComponent);
	pushRequiredComponent(ComponentIdentifier::Body2dComponent);
	initialize_all_particle_systems();
}


ThorParticleSystem::~ThorParticleSystem()
{
}

void ThorParticleSystem::initialize_all_particle_systems()
{
	const sf::Texture& star_texture = *m_textures_str_manager.loadTextures(
		Constant::texture_particle_dir + "particleCartoonStar.png");

	m_particle_systems_map["Cartoon_Star"].setTexture(star_texture);
}

void ThorParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& pair : m_particle_systems_map)
		target.draw(pair.second, states);
}

void ThorParticleSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	ParticleEmitterComponent* particle_emitter_comp = entity->getComp<ParticleEmitterComponent>();
	Body2dComponent* body2d_comp = entity->getComp<Body2dComponent>();

	sf::Vector2f entity_world_pos = body2d_comp->getSfWorldPosition();

	for (auto& emitter_data : particle_emitter_comp->m_emitters_list)
	{
		if (emitter_data.m_connection.isConnected())
			emitter_data.m_life_time -= dt;

		bool is_emitter_still_alive = (emitter_data.m_is_infinite_life_time 
			|| emitter_data.m_life_time > sf::Time::Zero);

		
		if (!is_emitter_still_alive || emitter_data.m_pause_emitter)
		{
			if (emitter_data.m_connection.isConnected())
				emitter_data.m_connection.disconnect();

			continue;
		}

		if (is_emitter_still_alive && !emitter_data.m_pause_emitter 
			&& !emitter_data.m_connection.isConnected())
		{
			auto find_system_iter = m_particle_systems_map.find(
				emitter_data.m_particle_system_name);

			if (find_system_iter == m_particle_systems_map.end())
				continue;

			emitter_data.m_connection = find_system_iter->second.addEmitter(
				thor::refEmitter(emitter_data.m_emitter));
		}

		emitter_data.m_emitter.setParticlePosition(thor::Distributions::circle(
			entity_world_pos + emitter_data.m_local_pos, emitter_data.m_local_radius));
	}
}


void ThorParticleSystem::finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt)
{
	for (auto& pair : m_particle_systems_map)
		pair.second.update(dt);
}
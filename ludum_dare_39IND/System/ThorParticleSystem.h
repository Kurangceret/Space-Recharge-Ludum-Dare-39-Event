#pragma once
#include "SingleEntitySystem.h"
#include <unordered_map>
#include <string>
#include <Thor\Particles\ParticleSystem.hpp>
#include "../Resource/TextureStringManager.h"


class ThorParticleSystem :	
	public SingleEntitySystem<ThorParticleSystem>
{
public:
	ThorParticleSystem(TextureStringManager& textures_str_manager);
	virtual ~ThorParticleSystem();
	void draw(sf::RenderTarget& target, sf::RenderStates states) override;
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;
	void finalizingTheUpdateCycle(EntityManager* entityManager, sf::Time dt) override;
private:
	void initialize_all_particle_systems();
private:
	std::unordered_map<std::string, thor::ParticleSystem> m_particle_systems_map;

	TextureStringManager& m_textures_str_manager;
};


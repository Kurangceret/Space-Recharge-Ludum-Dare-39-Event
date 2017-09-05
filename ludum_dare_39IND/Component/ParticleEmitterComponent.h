#pragma once
#include "Component.h"
#include <Thor\Particles\Emitters.hpp>
#include <Thor\Input\Connection.hpp>
#include <Thor\Math\Distributions.hpp>
#include <vector>
#include <string>

class ThorParticleSystem;
class ComponentArranger;

class ParticleEmitterComponent :	public Component{
public:
	friend class ThorParticleSystem;
	friend class ComponentArranger;
public:
	struct EmitterData{
		EmitterData(const std::string& particle_system_name,
			const sf::Time& life_time, bool is_infinite_life_time)
		:
		m_particle_system_name(particle_system_name), 
		m_emitter(),
		m_connection(),
		m_life_time(life_time),
		m_is_infinite_life_time(is_infinite_life_time),
		m_local_pos(),
		m_local_radius(1.f),
		m_particle_main_dir(),
		m_dir_angle_deviation(30.f),
		m_pause_emitter(false)
		{};

		sf::Vector2f m_local_pos;
		float m_local_radius;

		std::string m_particle_system_name;
		thor::UniversalEmitter m_emitter;
		thor::Connection m_connection;

		bool m_is_infinite_life_time;

		sf::Time m_life_time;

		float m_particle_magnitude;
		sf::Vector2f m_particle_main_dir;
		float m_dir_angle_deviation;

		bool m_pause_emitter;

	};
public:
	ParticleEmitterComponent(Entity* entity);
	virtual ~ParticleEmitterComponent();

	void pause_all_emitter();
	void continue_all_emitter();

	void set_all_emitter_direction(const sf::Vector2f& new_dir);
private:
	std::vector<EmitterData> m_emitters_list;
};


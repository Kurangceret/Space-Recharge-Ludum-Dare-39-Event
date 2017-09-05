#include "PlanetSpawnSystem.h"
#include "../Entity/EntityManager.h"
#include "../Entity/Entity.h"
#include "../Component/Body2dComponent.h"
#include "../Helper/Utility.h"
#include "../Helper/Math.h"
#include "../Component/AI/ScriptAIComponent.h"

#include "../Event/InvasionEvent.h"
#include "../Event/EventManager.h"
#include "../Audio/AudioManager.h"

PlanetSpawnSystem::PlanetSpawnSystem()
:m_cur_attack_level(0),
m_cur_invasion_level(0),
m_elap_attack_time(sf::Time::Zero),
m_elap_invasion_time(sf::Time::Zero),
m_attack_time(sf::seconds(18.f)),
m_invasion_time(sf::seconds(80.f))
{
	pushRequiredComponent(ComponentIdentifier::PlanetSpawnComponent);
	initialize_data();
}


PlanetSpawnSystem::~PlanetSpawnSystem()
{
}


void PlanetSpawnSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	update_attack_player_spawn(entityManager, entity, dt);
	update_invasion_spawn(entityManager, entity, dt);
}


void PlanetSpawnSystem::initialize_data()
{
	const std::string type1 = "Type1SpaceShip";
	const std::string type2 = "Type2SpaceShip";


	m_cur_invasion_spawn_box[0].m_spawns_list.push_back(SpawnData(type1, 6));
	m_cur_invasion_spawn_box[0].m_spawns_list.push_back(SpawnData(type2, 1));
	m_cur_invasion_spawn_box[0].m_count_to_advance = 2;

	m_cur_invasion_spawn_box[1].m_spawns_list.push_back(SpawnData(type1, 8));
	m_cur_invasion_spawn_box[1].m_spawns_list.push_back(SpawnData(type2, 2));
	m_cur_invasion_spawn_box[1].m_count_to_advance = 3;

	m_cur_invasion_spawn_box[2].m_spawns_list.push_back(SpawnData(type1, 10));
	m_cur_invasion_spawn_box[2].m_spawns_list.push_back(SpawnData(type2, 3));
	m_cur_invasion_spawn_box[2].m_count_to_advance = 3;


	m_cur_attack_player_spawn_box[0].m_spawns_list.push_back(SpawnData(type1, 2));
	//m_cur_attack_player_spawn_box[0].m_spawns_list.push_back(SpawnData(type2, 0));
	m_cur_attack_player_spawn_box[0].m_count_to_advance = 2;

	m_cur_attack_player_spawn_box[1].m_spawns_list.push_back(SpawnData(type1, 2));
	m_cur_attack_player_spawn_box[1].m_spawns_list.push_back(SpawnData(type2, 1));
	m_cur_attack_player_spawn_box[1].m_count_to_advance = 5;


	m_cur_attack_player_spawn_box[2].m_spawns_list.push_back(SpawnData(type1, 3));
	m_cur_attack_player_spawn_box[2].m_spawns_list.push_back(SpawnData(type2, 1));
	m_cur_attack_player_spawn_box[2].m_count_to_advance = 6;

	m_cur_attack_player_spawn_box[3].m_spawns_list.push_back(SpawnData(type1, 4));
	m_cur_attack_player_spawn_box[3].m_spawns_list.push_back(SpawnData(type2, 1));
	m_cur_attack_player_spawn_box[3].m_count_to_advance = 4;
}


void PlanetSpawnSystem::update_invasion_spawn(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	m_elap_invasion_time += dt;

	EventManager::getInstance()->queueEvent<InvasionEvent>(m_invasion_time - m_elap_invasion_time);

	if (m_elap_invasion_time >= m_invasion_time)
	{
		m_elap_invasion_time -= m_invasion_time;

		LevelData& level_data = m_cur_invasion_spawn_box[m_cur_invasion_level];

		level_data.m_cur_count++;
		if (level_data.m_cur_count >= level_data.m_count_to_advance)
		{
			m_cur_invasion_level++;
			std::size_t spawn_box_size = m_cur_invasion_spawn_box.size();
			if (m_cur_invasion_level >= spawn_box_size)
			{
				m_cur_invasion_level = spawn_box_size - 1;
			}

		}

		spawn_space_ships(entityManager, entity,level_data.m_spawns_list, "AttackPlanetAI.lua", "AttackPlanet");
		AudioManager::getInstance()->playSoundEffect("Invasion", 10);
	}
}


void PlanetSpawnSystem::update_attack_player_spawn(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	m_elap_attack_time += dt;

	if (m_elap_attack_time >= m_attack_time)
	{
		m_elap_attack_time -= m_attack_time;

		LevelData& level_data = m_cur_attack_player_spawn_box[m_cur_invasion_level];

		level_data.m_cur_count++;
		if (level_data.m_cur_count >= level_data.m_count_to_advance)
		{
			m_cur_attack_level++;
			std::size_t spawn_box_size = m_cur_attack_player_spawn_box.size();
			if (m_cur_attack_level >= spawn_box_size)
			{
				m_cur_attack_level = spawn_box_size - 1;
			}

		}

		spawn_space_ships(entityManager, entity, level_data.m_spawns_list, "AttackPlayerAI.lua", "AttackPlayer");
	}
}


void PlanetSpawnSystem::spawn_space_ships(EntityManager* ent_manager, Entity* planet_ent,
	const std::vector<SpawnData>& spawn_list, const std::string& state_file_name,
	const std::string& state_table)
{
	sf::Vector2f center_pos = planet_ent->getComp<Body2dComponent>()->getSfWorldPosition();
	for (const auto& spawn_data : spawn_list)
	{		
		for (int i = 0; i < spawn_data.m_count; i++)
		{
			sf::Vector2f random_pos = get_random_spawn_pos(center_pos);
			QueueEntityData::EngineInitializeFunc initialize_entity =
				[=](Entity* ent) -> void
			{
				Body2dComponent* ent_body2d_comp = ent->getComp<Body2dComponent>();

				ent_body2d_comp->setPosition(random_pos);

				ent->getComp<ScriptAIComponent>()->set_new_state_script(state_file_name, state_table);

			};

			ent_manager->queueEntityFromEngine(ent_manager->layerToZIndex("enemy_layer_1"), "",
				spawn_data.m_ent_name, true, &initialize_entity);
		}
	}
}

sf::Vector2f PlanetSpawnSystem::get_random_spawn_pos(const sf::Vector2f& center) const
{
	int min_radius = 120;
	int max_radius = 330;
	
	float random_radius = static_cast<float>(Utility::randomRange(min_radius, max_radius));
	float random_degree = Utility::randomRange(90, 180);

	
	sf::Vector2f random_dir = Math::degreeToVector(random_degree);

	return center + (random_dir * random_radius);
}
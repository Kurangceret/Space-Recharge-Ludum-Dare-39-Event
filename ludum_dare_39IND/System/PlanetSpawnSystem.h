#pragma once
#include "SingleEntitySystem.h"
#include <vector>
#include <string>
#include <unordered_map>

class PlanetSpawnSystem :	public SingleEntitySystem<PlanetSpawnSystem>{
public:
	struct SpawnData{
		SpawnData(const std::string& ent, std::size_t count)
		:m_ent_name(ent), m_count(count){}
		std::string m_ent_name;
		std::size_t m_count;
	};

	struct LevelData{
		LevelData():m_cur_count(0), m_count_to_advance(1){};
		LevelData(std::size_t advance_count) 
		:m_cur_count(0),
		m_count_to_advance(advance_count){}

		std::size_t m_cur_count;
		std::size_t m_count_to_advance;

		std::vector<SpawnData> m_spawns_list;
	};
	
public:
	PlanetSpawnSystem();
	virtual ~PlanetSpawnSystem();
protected:
	void processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt) override;

	
private:
	void initialize_data();
	void update_invasion_spawn(EntityManager* entityManager, Entity* entity, sf::Time dt);
	void update_attack_player_spawn(EntityManager* entityManager, Entity* entity, sf::Time dt);

	sf::Vector2f get_random_spawn_pos(const sf::Vector2f& center) const;

	void spawn_space_ships(EntityManager* ent_manager, Entity* planet_ent, 
		const std::vector<SpawnData>& spawn_list, const std::string& state_file_name, 
		const std::string& state_table);
private:
	sf::Time m_elap_invasion_time;

	sf::Time m_invasion_time;


	std::size_t m_cur_invasion_level;
	std::unordered_map<std::size_t, LevelData> m_cur_invasion_spawn_box;

	std::size_t m_cur_attack_level;
	std::unordered_map<std::size_t, LevelData> m_cur_attack_player_spawn_box;

	sf::Time m_elap_attack_time;
	sf::Time m_attack_time;
};


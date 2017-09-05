#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>

class EntityManager;
class ComponentArranger;

struct lua_State;

class SpaceShipComponent :	public Component{

public:
	friend class ComponentArranger;
public:
	struct Projectile_Data{
		std::string m_projectile_name;
		std::size_t m_target_category;
	};
public:
	SpaceShipComponent(Entity* entity);
	virtual ~SpaceShipComponent();

	//script usage
	bool has_missile_ability() const;
	bool can_shoot_missile() const;
	
	void allow_missile_to_shoot();
	void reset_missile_allowance();

	void shoot_new_projectile(EntityManager* entity_manager, 
		const std::string& name, const sf::Vector2f& initial_pos,
		const sf::Vector2f& dir, float projectile_speed);

	void shoot_new_projectile_guided(EntityManager* entity_manager,
		const std::string& name, const sf::Vector2f& initial_pos,
		const sf::Vector2f& dir, float projectile_speed, Entity* target_ent, lua_State* lua_state);

	bool has_projectile(const std::string& name) const;
private:
	const Projectile_Data* get_projectile_data(const std::string& name) const;
private:
	std::vector<Projectile_Data> m_projectiles_list;

	bool m_has_missile;

	bool m_can_shoot_missile;
};


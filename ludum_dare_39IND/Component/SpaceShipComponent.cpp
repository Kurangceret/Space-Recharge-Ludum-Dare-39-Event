#include "SpaceShipComponent.h"
#include "ComponentIdentifier.h"
#include "../Entity/EntityManager.h"
#include "../Component/Body2dComponent.h"
#include "../Helper/Math.h"
#include "../Component/ScriptUpdateComponent.h"
#include <Box2D\Dynamics\b2Fixture.h>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

SpaceShipComponent::SpaceShipComponent(Entity* entity)
:Component(entity, ComponentIdentifier::SpaceShipComponent),
m_has_missile(false),
m_can_shoot_missile(false)
{
}


SpaceShipComponent::~SpaceShipComponent()
{
}

bool SpaceShipComponent::has_missile_ability() const
{
	return m_has_missile;
}

bool SpaceShipComponent::can_shoot_missile() const
{
	return m_can_shoot_missile;
}

void SpaceShipComponent::reset_missile_allowance()
{
	m_can_shoot_missile = false;
}

void SpaceShipComponent::allow_missile_to_shoot()
{
	m_can_shoot_missile = true;
}

bool SpaceShipComponent::has_projectile(const std::string& name) const
{
	return (get_projectile_data(name) != nullptr);
}

const SpaceShipComponent::Projectile_Data* SpaceShipComponent::get_projectile_data(const std::string& name) const
{
	const Projectile_Data* final_data = nullptr;
	for (const auto& data : m_projectiles_list){
		if (data.m_projectile_name == name){
			final_data = &data;
			break;
		}
	}

	return final_data;
}

void SpaceShipComponent::shoot_new_projectile(EntityManager* entity_manager,
	const std::string& name, const sf::Vector2f& initial_pos,
	const sf::Vector2f& dir, float projectile_speed)
{
	if (!entity_manager)
		return;


	const Projectile_Data* projectile_data = get_projectile_data(name);

	if (!projectile_data)
		return;


	Entity* owner_entity = getOwnerEntity();

	QueueEntityData::EngineInitializeFunc initialize_entity = 
		[=](Entity* ent) -> void
	{
		Body2dComponent* ent_body2d_comp = ent->getComp<Body2dComponent>();

		const FixtureLogic& fixture_logic = ent_body2d_comp->getConstFixtureLogic("MainFixture");
		
		b2Filter filter = fixture_logic.mFixture->GetFilterData();

		filter.maskBits |= projectile_data->m_target_category;

		fixture_logic.mFixture->SetFilterData(filter);

		ent_body2d_comp->setBaseSpeed(projectile_speed);
		ent_body2d_comp->setPosition(initial_pos);
		ent_body2d_comp->setVelocity(dir, true);
		ent_body2d_comp->setAngleDegree(Math::vectorToDegree(dir));
	};
	
	entity_manager->queueEntityFromEngine(entity_manager->layerToZIndex("projectile_layer_1"), "", 
		projectile_data->m_projectile_name, true, &initialize_entity);
}

void SpaceShipComponent::shoot_new_projectile_guided(EntityManager* entity_manager,
	const std::string& name, const sf::Vector2f& initial_pos,
	const sf::Vector2f& dir, float projectile_speed, Entity* target_ent, lua_State* lua_state)
{
	if (!entity_manager)
		return;


	const Projectile_Data* projectile_data = get_projectile_data(name);

	if (!projectile_data)
		return;


	Entity* owner_entity = getOwnerEntity();

	QueueEntityData::EngineInitializeFunc initialize_entity =
		[=](Entity* ent) -> void
	{
		Body2dComponent* ent_body2d_comp = ent->getComp<Body2dComponent>();

		luabridge::LuaRef member_t =
			ent->getComp<ScriptUpdateComponent>()->getScriptMemberVariables(lua_state);

		member_t["tracked_entity"] = target_ent;

		const FixtureLogic& fixture_logic = ent_body2d_comp->getConstFixtureLogic("MainFixture");

		b2Filter filter = fixture_logic.mFixture->GetFilterData();

		filter.maskBits |= projectile_data->m_target_category;

		fixture_logic.mFixture->SetFilterData(filter);

		ent_body2d_comp->setBaseSpeed(projectile_speed);
		ent_body2d_comp->setPosition(initial_pos);
		ent_body2d_comp->setVelocity(sf::Vector2f(), true);
		ent_body2d_comp->setAngleDegree(Math::vectorToDegree(dir));
	};

	entity_manager->queueEntityFromEngine(entity_manager->layerToZIndex("projectile_layer_1"), "",
		projectile_data->m_projectile_name, true, &initialize_entity);
}
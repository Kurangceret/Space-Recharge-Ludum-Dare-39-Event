#pragma once
#include "BaseStateEvent.h"
#include <string>
#include "../GUI/MeterBar.h"
#include "../GUI/CircleBar.h"
#include "../GUI/OrderSpriteBar.h"
#include "../Component/HealthComponent.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class PlayerComponent;
class JumpComponent;
class RealTimeInputComponent;
class Body2dComponent;
class Entity;
class AutoBody2dMovementComponent;
class AutomaticPathComponent;
class EntityManager;
class AnimationComponent;

class EntityCollidesEntityEvent;
class SpaceShipComponent;

class TextureStringManager;
class HealthComponent;
class InvasionEvent;
struct lua_State;

class StandardStateEvent :	public BaseStateEvent{
public:
	enum State{
		Normal,
		Dash,
		Recharge,
		Dead,
		Rebuilding,
		Ability,
		StateCount
	};
public:
	StandardStateEvent(StateEventManager* stateEventManager, 
		const sf::Vector2f& gravityDir = sf::Vector2f(0.f, 1.f));
	virtual ~StandardStateEvent();


	void initializeStateEvent(lua_State* luaState) override;
protected:
	void updateOnGameState(GeneralData* generalData,
		sf::RenderWindow& renderWindow,
		sf::Time dt) override;

	void handleGameStateEvent(GeneralData* generalData,
		sf::RenderWindow& renderWindow,
		const sf::Event& event) override;

	void drawOnGuiGameState(sf::RenderTarget& renderTarget,
		sf::RenderStates states) override;

	void updateOnGuiGameState(GuiGameStateData* guiGameStateData,
		sf::RenderWindow& renderWindow,
		sf::Time dt) override;
private:
	void initializePlayerMemberData(Entity* playerEnt, GeneralData* generalData);

	

	void update_movement(GeneralData* generalData,
		sf::RenderWindow& renderWindow,
		sf::Time dt);

	void trigger_first_ability(EntityManager* entity_manager);
	void trigger_second_ability(EntityManager* entity_manager);

	void trigger_dash();

	void trigger_recharge();

	void update_power_usage(sf::Time dt);

	void handle_invasion_event(const InvasionEvent* invasion_event);

private:
	void initialize_ui(sf::RenderWindow& renderWindow);

	void update_ability_ui(sf::RenderTarget& renderWindow);
	void update_planet_ui(sf::RenderTarget& renderWindow);
	void update_clock_ui(sf::RenderTarget& renderWindow);
	void update_energy_ui(sf::RenderTarget& renderWindow);
	void update_space_button_ui(sf::RenderTarget& renderWindow);
	void update_planet_arrow_ui(sf::RenderTarget& renderWindow);
	void update_health_ui(sf::RenderTarget& renderWindow);
private:
	Entity* m_player_ent;
	Body2dComponent* m_player_body2d_comp;
	PlayerComponent* m_player_comp;
	RealTimeInputComponent* m_real_time_input_comp;
	AnimationComponent* m_player_anim_comp;
	AutomaticPathComponent* m_player_path_comp;
	SpaceShipComponent* m_space_ship_comp;





	lua_State* m_lua_state;
	
	sf::Time m_cur_first_ability_cooldown;
	sf::Time m_first_ability_cooldown;

	sf::Time m_cur_second_ability_cooldown;
	sf::Time m_second_ability_cooldown;
	
	float m_player_speed;

	sf::Time m_cur_dash_time;
	float m_dash_speed;
	sf::Time m_dash_time;
	float m_dash_power_percent_usage;

	float m_laser_shoot_gap;
	float m_laser_proj_speed;


	float m_color_laser_shoot_gap;
	float m_color_laser_proj_speed;

		



	Entity* m_planet_earth_ent;
	Body2dComponent* m_planet_earth_body2d_comp;
	HealthComponent* m_planet_earth_health_comp;
	std::size_t m_planet_earth_category;



	Entity* m_planet_enemy_ent;
	Body2dComponent* m_planet_enemy_body2d_comp;
	HealthComponent* m_planet_enemy_health_comp;
	std::size_t m_planet_enemy_category;


	sf::Time m_recharge_time;
	sf::Time m_cur_recharge_time;

	State m_state;
	//UI related

	sf::Font m_font;

	TextureStringManager* m_textures_str;

	sf::Sprite m_ability1_icon;

	sf::Text m_ability2_text_ui;
	sf::Sprite m_ability2_icon;


	sf::Sprite m_earth_ui;
	sf::Text m_earth_text_ui;

	sf::Sprite m_planet_enemy_ui;
	sf::Text m_planet_enemy_text_ui;

	sf::Sprite m_clock_ui;
	sf::Text m_text_clock_ui;


	sf::Sprite m_outline_energy_bar_ui;
	sf::Sprite m_energy_bar_ui;
	sf::IntRect m_energy_bar_int_rect;


	sf::Sprite m_space_button_ui;

	sf::Sprite m_earth_arrow_ui;
	sf::Sprite m_planet_enemy_arrow_ui;

	sf::Sprite m_health_spaceship_ui;

	GeneralData* m_general_data;

	sf::Time m_wait_death_time;
	sf::Time m_cur_wait_death_time;

	


};


#include "StandardStateEvent.h"
#include "../System/AllSystemsHeader.h"
#include "../WindowState/GeneralData.h"

#include "../Component/Body2dComponent.h"
#include "../Component/PlayerComponent.h"
#include "../Component/AI/SteeringBehavior.h"

#include "../Helper/Math.h"
#include "../Helper/Utility.h"
#include "../Helper/Constant.h"
#include "../Component/EntityParentComponent.h"
#include "../Component/AutoBody2dMovementComponent.h"
#include "../Component/AutomaticPathComponent.h"
#include "../Component/JumpComponent.h"

#include "../Component/RealTimeInputComponent.h"

#include "../Component/AnimationComponent.h"
#include "../Component/CategoryComponent.h"

#include "../Component/SpaceShipComponent.h"

#include "../Resource/TextureStringManager.h"
#include "../Event/EventManager.h"
#include "../Audio/AudioManager.h"
#include "../Event/InvasionEvent.h"

#include "../Resource/TextureStringManager.h"

StandardStateEvent::StandardStateEvent(StateEventManager* stateEventManager,
	const sf::Vector2f& gravityDir)
:BaseStateEvent(stateEventManager, StateEventsIdentifier::Standard),
m_player_ent(nullptr),
m_player_speed(100.0),
m_cur_first_ability_cooldown(sf::Time::Zero),
m_cur_second_ability_cooldown(sf::Time::Zero),
m_state(State::Normal)
{
}


StandardStateEvent::~StandardStateEvent()
{
	EventManager::getInstance()->deleteListener<InvasionEvent>
		(Utility::convertPointerToAddress(this));
}


void StandardStateEvent::initializeStateEvent(lua_State* luaState)
{
	Utility::readLuaScriptFile(Constant::stateEventScriptDir + "StandardStateEvent.lua", luaState);
	luabridge::LuaRef stateTable = luabridge::LuaRef::fromStack(luaState, -1);

	m_lua_state = luaState;
	m_player_speed = stateTable["player_speed"].cast<float>();
	m_laser_proj_speed = stateTable["laser_proj_speed"].cast<float>();
	m_laser_shoot_gap = stateTable["laser_shoot_gap"].cast<float>();


	m_first_ability_cooldown = sf::seconds(stateTable["first_ability_cooldown"].cast<float>());
	m_second_ability_cooldown = sf::seconds(stateTable["second_ability_cooldown"].cast<float>());

	m_dash_speed = stateTable["dash_speed"].cast<float>();
	m_dash_time = sf::seconds(stateTable["dash_time"].cast<float>());
	m_dash_power_percent_usage = stateTable["dash_power_percent_usage"].cast<float>();

	m_color_laser_shoot_gap = stateTable["color_laser_shoot_gap"].cast<float>();
	m_color_laser_proj_speed = stateTable["color_laser_proj_speed"].cast<float>();

	m_planet_enemy_category = stateTable["planet_enemy_category"].cast<std::size_t>();
	m_planet_earth_category = stateTable["planet_earth_category"].cast<std::size_t>();

	m_recharge_time = sf::seconds(stateTable["recharge_time"].cast<float>());

	m_wait_death_time = sf::seconds(stateTable["wait_death_time"].cast<float>());

	BaseStateEvent::initializeStateEvent(luaState);
}

void StandardStateEvent::updateOnGameState(GeneralData* generalData,
	sf::RenderWindow& renderWindow,
	sf::Time dt)
{
	if (!m_player_ent){
		initializePlayerMemberData(generalData->mPlayerEntity, generalData);
		initialize_ui(renderWindow);
		return;
	}

	update_power_usage(dt);

	if (m_planet_earth_health_comp->getCurrentHealth() <= 0.f)
	{
		generalData->mIsGameOver = true;
		generalData->m_game_over_message =
			"You lost!\nYour home planet was destroyed by enemies!";
	}

	if (m_planet_enemy_health_comp->getCurrentHealth() <= 0.f)
	{
		generalData->mIsGameOver = true;
		generalData->m_game_over_message =
			"Vicotry!\nYou have destroyed the enemies planet!";
	}

	if (m_state == State::Normal)
	{
		m_player_anim_comp->setAnimation("Idle");
		if (m_player_comp->m_cur_power <= 0.f){
			m_cur_wait_death_time = m_wait_death_time;
			m_state = State::Dead;
			generalData->m_player_health--;
			AudioManager::getInstance()->playSoundEffect("SpaceShip_Explode", 20);

			if (generalData->m_player_health <= 0){
				generalData->mIsGameOver = true;
				generalData->m_game_over_message = 
					"You lost!\nAll of the three spaceships were destroyed by enemies!";
			}
		}
	}

	if (m_state == State::Dead)
	{
		m_player_comp->m_is_player_available = false;
		m_player_anim_comp->setAnimation("Destroyed");
		m_player_body2d_comp->setVelocity(0.f, 0.f, false);
		m_cur_wait_death_time -= dt;

		if (m_cur_wait_death_time <= sf::Time::Zero){
			sf::Vector2f planet_earth_pos = m_planet_earth_body2d_comp->getSfWorldPosition();
			sf::Vector2f final_pos = planet_earth_pos + (Math::degreeToVector(315.0) * 300.f);
			m_player_body2d_comp->setPosition(final_pos);
			m_player_comp->m_cur_drain_power_rate = m_player_comp->m_min_drain_power_rate;
			m_player_comp->m_cur_power = m_player_comp->m_max_power;
			m_state = State::Normal;
			m_player_comp->m_is_player_available = true;
		}
	}
	

	if (m_state == State::Recharge)
	{
		m_cur_recharge_time -= dt;
		if (m_cur_recharge_time <= sf::Time::Zero)
		{
			sf::Vector2f planet_earth_pos = m_planet_earth_body2d_comp->getSfWorldPosition();
			sf::Vector2f final_pos = planet_earth_pos + (Math::degreeToVector(315.0) * 300.f);
			m_player_body2d_comp->setPosition(final_pos);
			m_player_comp->m_cur_drain_power_rate = m_player_comp->m_min_drain_power_rate;
			m_player_comp->m_cur_power = m_player_comp->m_max_power;

			m_state = Normal;
		}
	}
	
	if (m_cur_first_ability_cooldown > sf::Time::Zero){
		m_cur_first_ability_cooldown -= dt;
	}

	if (m_cur_second_ability_cooldown > sf::Time::Zero){
		m_cur_second_ability_cooldown -= dt;
	}
	
	if (m_state == State::Dash)
	{
		m_player_anim_comp->setAnimation("Dash");
		m_cur_dash_time -= dt;
		if (m_cur_dash_time <= sf::Time::Zero){
			m_state = State::Normal;
			m_player_body2d_comp->setBaseSpeed(m_player_speed);
		}

	}
	

	
	update_movement(generalData, renderWindow, dt);
	if (m_state != Recharge && m_state != Dead)
		BaseStateEvent::CameraOnPlayerFunc(generalData, renderWindow, dt);
}

void StandardStateEvent::update_power_usage(sf::Time dt)
{
	if (m_state != Normal)
		return;

	m_player_comp->m_cur_power -= (m_player_comp->m_cur_drain_power_rate * dt.asSeconds());
}

void StandardStateEvent::update_movement(GeneralData* generalData,
	sf::RenderWindow& renderWindow,
	sf::Time dt)
{
	if (m_state != State::Normal && m_state != State::Ability)
		return;

	sf::Vector2f mouse_world_pos =
		renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));

	sf::Vector2f player_world_pos = m_player_body2d_comp->getSfWorldPosition();

	sf::Vector2f player_to_mouse_dir = Math::unitVector(mouse_world_pos - player_world_pos);



	sf::Vector2f new_dir = SteeringBehavior::seek(m_player_body2d_comp,
		player_to_mouse_dir, dt);

	m_player_body2d_comp->setVelocity(new_dir * m_player_speed, false);
}

void StandardStateEvent::handle_invasion_event(const InvasionEvent* invasion_event)
{
	int val = static_cast<int>(invasion_event->m_cur_invasion_time.asSeconds());
	m_text_clock_ui.setString(std::to_string(val));
}

void StandardStateEvent::handleGameStateEvent(GeneralData* generalData,
	sf::RenderWindow& renderWindow,
	const sf::Event& event)
{
	if (m_state == State::Recharge)
		return;

	if (!m_player_ent)
		return;

	sf::Vector2f mouse_world_pos = 
		renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));


	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left)
	{
		trigger_first_ability(generalData->mEntityManager.get());
	}

	if (event.type == sf::Event::MouseButtonPressed && 
		event.mouseButton.button == sf::Mouse::Right)
	{
		trigger_second_ability(generalData->mEntityManager.get());
	}

	if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Space)
	{
		if (m_player_comp->m_is_player_in_charge_area){
			trigger_recharge();
		}
		else{
			trigger_dash();
		}
	}

}

void StandardStateEvent::trigger_recharge()
{
	m_state = State::Recharge;

	m_player_body2d_comp->setPosition(sf::Vector2f(-400.f, -400.f));
	m_player_body2d_comp->setVelocity(0.f, 0.f, false);
	m_cur_recharge_time = m_recharge_time;
	AudioManager::getInstance()->playSoundEffect("Recharge", 20);
}


void StandardStateEvent::trigger_first_ability(EntityManager* entity_manager)
{
	if (m_cur_first_ability_cooldown > sf::Time::Zero)
		return;

	sf::Vector2f main_dir = Math::unitVector(m_player_body2d_comp->getFacingDir());
	sf::Vector2f player_pos = m_player_body2d_comp->getSfWorldPosition();

	sf::Vector2f down_dir = Math::down_perpendicular(main_dir);
	sf::Vector2f up_dir = Math::up_perpendicular(main_dir);

	sf::Vector2f down_pos = player_pos + (down_dir * m_laser_shoot_gap);
	sf::Vector2f up_pos = player_pos + (up_dir * m_laser_shoot_gap);


	m_space_ship_comp->shoot_new_projectile(entity_manager, "SpaceShipLaser",
		player_pos, main_dir, m_laser_proj_speed);
	m_space_ship_comp->shoot_new_projectile(entity_manager, "SpaceShipLaser",
		down_pos, main_dir, m_laser_proj_speed);
	m_space_ship_comp->shoot_new_projectile(entity_manager, "SpaceShipLaser",
		up_pos, main_dir, m_laser_proj_speed);

	m_cur_first_ability_cooldown = m_first_ability_cooldown;

	AudioManager::getInstance()->playSoundEffect("Laser_SpaceShip2", 30);
}

void StandardStateEvent::trigger_second_ability(EntityManager* entity_manager)
{
	if (m_cur_second_ability_cooldown > sf::Time::Zero)
		return;

	sf::Vector2f player_pos = m_player_body2d_comp->getSfWorldPosition();
	float starting_angle = 0.f;

	while (starting_angle < 360.f)
	{
		sf::Vector2f main_dir = Math::degreeToVector(starting_angle);

		m_space_ship_comp->shoot_new_projectile(entity_manager, "SpaceShipColorLaser",
			player_pos, main_dir, m_color_laser_proj_speed);

		starting_angle += m_color_laser_shoot_gap;
	}
	m_cur_second_ability_cooldown = m_second_ability_cooldown;

	AudioManager::getInstance()->playSoundEffect("Ability_2nd", 30);
}

void StandardStateEvent::trigger_dash()
{
	float usage_power = m_dash_power_percent_usage * m_player_comp->m_max_power;
	if (m_player_comp->m_cur_power < usage_power)
		return;


	m_cur_dash_time = m_dash_time;
	m_player_body2d_comp->setBaseSpeed(m_dash_speed);
	m_player_body2d_comp->setVelocity(m_player_body2d_comp->getFacingDir(), true);
	m_player_comp->m_cur_power -= usage_power;
	m_state = State::Dash;
	AudioManager::getInstance()->playSoundEffect("Dash", 30);
}

void StandardStateEvent::initializePlayerMemberData(Entity* playerEnt, GeneralData *generalData)
{
	EventManager::getInstance()->addListener <InvasionEvent>
		(Utility::convertPointerToAddress(this),
		std::bind(&StandardStateEvent::handle_invasion_event,
		this, std::placeholders::_1));

	m_general_data = generalData;
	m_player_ent = playerEnt;
	m_player_comp = m_player_ent->getComp<PlayerComponent>();
	m_player_body2d_comp = m_player_ent->getComp<Body2dComponent>();
	m_real_time_input_comp = m_player_ent->getComp<RealTimeInputComponent>();
	m_player_path_comp = m_player_ent->getComp<AutomaticPathComponent>();
	m_space_ship_comp = m_player_ent->getComp<SpaceShipComponent>();
	m_player_anim_comp = m_player_ent->getComp<AnimationComponent>();

	m_textures_str = generalData->mTexturesStringmanager.get();

	EntityManager* ent_manager = generalData->mEntityManager.get();
	std::size_t planet_index = ent_manager->layerToZIndex("planet_layer_1");

	InteractionFuncData::InteractionFunc planet_earth_func = [&](Entity* entity)
	{
		if (entity->getComp<CategoryComponent>()->getCategory() & m_planet_earth_category)
		{
			m_planet_earth_ent = entity;
			m_planet_earth_body2d_comp = entity->getComp<Body2dComponent>();
			m_planet_earth_health_comp = entity->getComp<HealthComponent>();
		}

	};

	
	ent_manager->pushInteractionFuncData(InteractionFuncData(
		m_player_ent, planet_earth_func, true, planet_index));


	InteractionFuncData::InteractionFunc planet_enemy_func = [&](Entity* entity)
	{
		if (entity->getComp<CategoryComponent>()->getCategory() & m_planet_enemy_category)
		{
			m_planet_enemy_ent = entity;
			m_planet_enemy_body2d_comp = entity->getComp<Body2dComponent>();
			m_planet_enemy_health_comp = entity->getComp<HealthComponent>();
		}
	};


	ent_manager->pushInteractionFuncData(InteractionFuncData(
		m_player_ent, planet_enemy_func, true, planet_index));

	//mPlayerAnimComp = mPlayerEnt->getComp<AnimationComponent>();
		
	/*generalData->mEntityManager->pushInteractionFuncData(mPlayerEnt, 
		[this](Entity* entity)
	{
		std::size_t entCategory = entity->getComp<CategoryComponent>()->getCategory();
		if ((entCategory & mSheepCategory) != 0){
			mNumberOfSheepLeft++;
		}
		if ((entCategory & mOxCategory) != 0){
			mNumberOfOxLeft++;
		}
	}, true, "enemy_layer_1");*/

	
}


void StandardStateEvent::initialize_ui(sf::RenderWindow& renderWindow)
{
	m_font.loadFromFile("Media/Fonts/Sansation.ttf");

	m_ability2_text_ui.setFont(m_font);
	m_ability2_text_ui.setCharacterSize(32);
	m_ability2_text_ui.setColor(sf::Color::White);

	m_earth_text_ui.setFont(m_font);
	m_earth_text_ui.setCharacterSize(26);
	m_earth_text_ui.setColor(sf::Color::White);

	m_planet_enemy_text_ui.setFont(m_font);
	m_planet_enemy_text_ui.setCharacterSize(26);
	m_planet_enemy_text_ui.setColor(sf::Color::White);

	m_text_clock_ui.setFont(m_font);
	m_text_clock_ui.setCharacterSize(28);
	m_text_clock_ui.setColor(sf::Color::Red);


	sf::Color opacity_color_150(255, 255, 255, 150);
	sf::Color opacity_color_100(255, 255, 255, 100.f);

	m_ability1_icon.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "ability1_ui.png"));
	m_ability2_icon.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "ability2_ui.png"));
	m_ability1_icon.setColor(opacity_color_150);
	m_ability2_icon.setColor(opacity_color_150);

	m_earth_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "earth_ui.png"));
	m_planet_enemy_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "planet_ui.png"));

	m_clock_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "clock_ui.png"));

	m_outline_energy_bar_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "empty_bar_ui.png"));
	m_energy_bar_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "fullbar_ui.png"));
	m_energy_bar_int_rect = m_energy_bar_ui.getTextureRect();


	m_space_button_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "dash_ui.png"));

	m_earth_arrow_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "earth_arrow ui.png"));
	m_earth_arrow_ui.setColor(opacity_color_100);
	m_planet_enemy_arrow_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "planet_arrow_ui.png"));
	m_planet_enemy_arrow_ui.setColor(opacity_color_100);

	m_health_spaceship_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + "spaceship_ui.png"));

}


void StandardStateEvent::drawOnGuiGameState(sf::RenderTarget& renderTarget,
	sf::RenderStates states)
{
	if (!m_player_ent)
		return;

	update_ability_ui(renderTarget);
	update_planet_ui(renderTarget);
	update_clock_ui(renderTarget);
	update_energy_ui(renderTarget);
	update_space_button_ui(renderTarget);
	update_planet_arrow_ui(renderTarget);
	update_health_ui(renderTarget);
		

	renderTarget.draw(m_ability1_icon);
	renderTarget.draw(m_ability2_icon);

	renderTarget.draw(m_earth_ui);
	renderTarget.draw(m_planet_enemy_ui);

	renderTarget.draw(m_clock_ui);

	renderTarget.draw(m_space_button_ui);

	renderTarget.draw(m_outline_energy_bar_ui);
	renderTarget.draw(m_energy_bar_ui);

	renderTarget.draw(m_health_spaceship_ui);

	sf::View view = renderTarget.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f ori_pos = m_health_spaceship_ui.getPosition();
	for (int i = 1; i < m_general_data->m_player_health; i++)
	{
		sf::Vector2f add_pos(view_port.width * 0.08, 0.f);
		m_health_spaceship_ui.setPosition(m_health_spaceship_ui.getPosition() + add_pos);
		renderTarget.draw(m_health_spaceship_ui);
	}
	m_health_spaceship_ui.setPosition(ori_pos);


	renderTarget.draw(m_text_clock_ui);
	renderTarget.draw(m_earth_text_ui);
	renderTarget.draw(m_planet_enemy_text_ui);
	renderTarget.draw(m_ability2_text_ui);

	renderTarget.draw(m_earth_arrow_ui);
	renderTarget.draw(m_planet_enemy_arrow_ui);
}

void StandardStateEvent::updateOnGuiGameState(GuiGameStateData* guiGameStateData,
	sf::RenderWindow& renderWindow,
	sf::Time dt)
{
	if (!m_player_ent){
		
		return;
	}

	
}


void StandardStateEvent::update_ability_ui(sf::RenderTarget& renderWindow)
{
	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	

	sf::Vector2f pos_1(view_port.left + (view_port.width * 0.85), view_port.top + (view_port.height * 0.75));

	m_ability2_icon.setPosition(pos_1);

	sf::Vector2f text_pos(pos_1);
	text_pos.x += view_port.width * 0.05;
	text_pos.y += view_port.height * 0.035;
	
	std::string str = "";
	if (m_cur_second_ability_cooldown > sf::Time::Zero)
	{
		int val_int = static_cast<int>(m_cur_second_ability_cooldown.asSeconds());
		str = std::to_string(val_int);
	}

	m_ability2_text_ui.setString(str);
	m_ability2_text_ui.setPosition(text_pos);

	sf::Vector2f pos_2(pos_1);
	pos_2.y += m_ability2_icon.getTextureRect().height;

	m_ability1_icon.setPosition(pos_2);
}

void StandardStateEvent::update_planet_ui(sf::RenderTarget& renderWindow)
{
	if (!m_planet_earth_ent || !m_planet_enemy_ent)
		return;

	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f planet_earth_pos(view_port.left + (view_port.width * 0.0), 
		view_port.top + (view_port.height * 0.02));

	m_earth_ui.setPosition(planet_earth_pos);

	sf::Vector2f planet_enemy_pos(view_port.left + (view_port.width * 0.9), 
		view_port.top + (view_port.height * 0.02));

	m_planet_enemy_ui.setPosition(planet_enemy_pos);

	sf::Vector2f earth_text_pos(planet_earth_pos);
	earth_text_pos.x += view_port.width * 0.02;
	earth_text_pos.y += view_port.height * 0.05;


	float max_earth_hp = m_planet_earth_health_comp->getMaxHealth();
	float cur_earth_hp = m_planet_earth_health_comp->getCurrentHealth();

	float earth_percent = (cur_earth_hp / max_earth_hp) * 100.f;
	int earth_percent_int = static_cast<int>(earth_percent);

	m_earth_text_ui.setString(std::to_string(earth_percent_int) + "%");
	m_earth_text_ui.setPosition(earth_text_pos);


	sf::Vector2f enemy_text_pos(planet_enemy_pos);
	enemy_text_pos.x += view_port.width * 0.025;
	enemy_text_pos.y += view_port.height * 0.06;


	float max_enemy_hp = m_planet_enemy_health_comp->getMaxHealth();
	float cur_enemy_hp = m_planet_enemy_health_comp->getCurrentHealth();

	float enemy_percent = (cur_enemy_hp / max_enemy_hp) * 100.f;
	int enemy_percent_int = static_cast<int>(enemy_percent);

	m_planet_enemy_text_ui.setString(std::to_string(enemy_percent_int) + "%");
	m_planet_enemy_text_ui.setPosition(enemy_text_pos);

}

void StandardStateEvent::update_clock_ui(sf::RenderTarget& renderWindow)
{
	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f enemy_pos = m_planet_enemy_ui.getPosition();

	sf::Vector2f clock_pos(enemy_pos);
	clock_pos.y += m_planet_enemy_ui.getTextureRect().height + view_port.height * 0.02;
	clock_pos.x += view_port.width * 0.01;

	m_clock_ui.setPosition(clock_pos);

	sf::Vector2f text_pos(clock_pos);
	text_pos.x += view_port.width * 0.035;
	text_pos.y += view_port.height * 0.005;

	//m_text_clock_ui.setString("11");
	m_text_clock_ui.setPosition(text_pos);
}

void StandardStateEvent::update_energy_ui(sf::RenderTarget& renderWindow)
{
	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f pos(view_port.left + view_port.width * 0.45,
		view_port.top + view_port.height * 0.02);

	m_outline_energy_bar_ui.setPosition(pos);

	m_energy_bar_ui.setPosition(pos + sf::Vector2f(view_port.width * 0.001, view_port.height * 0.0025));

	sf::FloatRect rect(m_energy_bar_int_rect);

	rect.width = rect.width * (m_player_comp->m_cur_power / m_player_comp->m_max_power);

	m_energy_bar_ui.setTextureRect(sf::IntRect(rect));

}

void StandardStateEvent::update_space_button_ui(sf::RenderTarget& renderWindow)
{
	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f pos(view_port.left + view_port.width * 0.4,
		view_port.top + view_port.height * 0.93);

	std::string final_name = m_player_comp->m_is_player_in_charge_area ? "recharge_ui.png" : "dash_ui.png";

	
	m_space_button_ui.setTexture(*m_textures_str->loadTextures(Constant::texture_ui_dir + final_name));
	

	m_space_button_ui.setPosition(pos);
}

void StandardStateEvent::update_planet_arrow_ui(sf::RenderTarget& renderWindow)
{
	if (!m_player_ent || !m_planet_earth_ent || !m_planet_enemy_ent)
		return;

	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f player_pos = m_player_body2d_comp->getSfWorldPosition();

	sf::Vector2f earth_pos = m_planet_earth_body2d_comp->getSfWorldPosition();

	sf::Vector2f planet_enemy_pos = m_planet_enemy_body2d_comp->getSfWorldPosition();


	sf::Vector2f player_to_earth = Math::unitVector(earth_pos - player_pos);
	sf::Vector2f player_to_enemy = Math::unitVector(planet_enemy_pos - player_pos);

	sf::Vector2f earth_arrow_pos(view.getCenter());
	earth_arrow_pos += Math::multiplyVector(player_to_earth, (view.getSize() / 2.f));

	sf::Vector2f adjuster_pos = Math::multiplyVector(-player_to_earth, view.getSize() * 0.05f);
	earth_arrow_pos += adjuster_pos;

	m_earth_arrow_ui.setPosition(earth_arrow_pos);
	m_earth_arrow_ui.setRotation(Math::vectorToDegree(player_to_earth));

	sf::Vector2f enemy_arrow_pos(view.getCenter());
	enemy_arrow_pos += Math::multiplyVector(player_to_enemy, (view.getSize() / 2.f));

	adjuster_pos = Math::multiplyVector(-player_to_enemy, view.getSize() * 0.05f);
	enemy_arrow_pos += adjuster_pos;

	m_planet_enemy_arrow_ui.setPosition(enemy_arrow_pos);
	m_planet_enemy_arrow_ui.setRotation(Math::vectorToDegree(player_to_enemy));

}


void StandardStateEvent::update_health_ui(sf::RenderTarget& renderWindow)
{
	sf::View view = renderWindow.getView();
	sf::FloatRect view_port(view.getCenter().x - view.getSize().x / 2.f,
		view.getCenter().y - view.getSize().y / 2.f, view.getSize().x, view.getSize().y);

	sf::Vector2f pos(view_port.left + view_port.width * 0.4,
		view_port.top + view_port.height * 0.07);

	m_health_spaceship_ui.setPosition(pos);
}
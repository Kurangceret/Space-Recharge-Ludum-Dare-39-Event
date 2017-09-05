--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
dofile(EngineConstant.ScriptDir .. "CategoryScript.lua")

local State = {
    player_speed = 280.0,
    laser_proj_speed = 600.0,
    laser_shoot_gap = 16.0,

    first_ability_cooldown = 0.5,
    second_ability_cooldown = 10.0,

    dash_speed = 760.0,
    dash_time = 0.4,
    dash_power_percent_usage = 0.05,
    
    color_laser_shoot_gap = 15,
    color_laser_proj_speed = 700.0,

    wait_death_time = 3.0,

    planet_enemy_category = Category.PlanetEnemy,
    planet_earth_category = Category.PlanetEarth,

    recharge_time = 5.0


}

return State


--endregion

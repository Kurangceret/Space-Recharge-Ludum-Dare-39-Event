dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")


AttackPlanetAIStates = {
  
    ChasePlanetState = 
    {
        name = "ChasePlanetState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAnimation():setAnimation("Idle")
            if (self.planet_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex(Constant.Planet_Layer), {},
                 
                    function(entity, interactEnt, luaData)
                        if(Utility.isBothIndexRelated(interactEnt:compCategory():getCategory(), Category.PlanetEarth)) then
                            self.planet_entity = interactEnt
                        end
                    end
                    )
            end

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            owner_auto_path_comp:clearAutomaticPath()

            

		end,

	    update = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAnimation():setAnimation("Idle")
            if(self.planet_entity == nil) then
                return
            end
                                               
           self.find_pos_to_shoot_planet(self, owner_entity, entity_manager, grid_map, dt)



		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
        end,

        find_pos_to_shoot_planet = function(self, owner_entity, entity_manager, grid_map)
            if(self.has_found_pos) then
                return
            end
            
            local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
            
            local off_set_num = EngineUtil.randomRange(0, 2)

            local dir_degree_deviation = EngineUtil.randomRange(self.min_dir_degree_deviation, self.max_dir_degree_deviation)

            
            local final_dir_degree = self.initial_dir_degree
            
            if (off_set_num == 1) then
                final_dir_degree = EngineMath.plusDegree(final_dir_degree, dir_degree_deviation)
            elseif (off_set_num == 0) then
                final_dir_degree = EngineMath.minusDegree(final_dir_degree, dir_degree_deviation)
            end

            local final_dir = EngineMath.degreeToVector(final_dir_degree)

            local random_dist_to_shoot = EngineUtil.randomRange(self.min_dist_to_shoot, self.max_dist_to_shoot)

            local final_off_set_pos = EngineMath.multiplyVectorByFloat(final_dir, random_dist_to_shoot)

            local final_pos = EngineMath.plusVector(planet_pos, final_off_set_pos)

            owner_entity:compAutomaticPath():setDestToCalculate(final_pos.x, final_pos.y)

            self.has_found_pos = true
        end,

        planet_entity = nil,

        has_found_pos = false,

        initial_dir_degree = 315.0,

        min_dir_degree_deviation = 10.0,
        max_dir_degree_deviation = 45.0,

        min_dist_to_shoot = 600.0,

        max_dist_to_shoot = 850.0,
        
        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    
                    if(self.has_found_pos ~= true) then
                        return false
                    end

                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                    if(owner_auto_path_comp:isUncalculatedPosValid() ~= true and
                        owner_auto_path_comp:isAutomaticPathEmpty()) then
                        return true
                    end
                    
                    return false
                end,			   

				callTargetState = function(owner_entity)
                    local aiScriptComp = owner_entity:compScriptAI()
                    aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlanetAIStates.AttackPlanetState))
                end

			}

        }

    },
    AttackPlanetState = 
    {
        name = "AttackPlayerState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            self.planet_entity = nil
            self.elapsed_time = 0.0
            self.is_type2 = owner_entity:compSpaceShip():has_projectile("UfoMissile")
            
            if (self.planet_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex(Constant.Planet_Layer), {},
                 
                    function(entity, interactEnt, luaData)
                        if(Utility.isBothIndexRelated(interactEnt:compCategory():getCategory(), Category.PlanetEarth)) then
                            self.planet_entity = interactEnt
                        end
                    end
                    )
            end

            if(self.is_type2) then

            end
              

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            owner_auto_path_comp:clearAutomaticPath()

            --self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)

		end,

	    update = function(self, owner_entity, entity_manager, grid_map, dt)
            self.elapsed_time = self.elapsed_time + dt 
            owner_entity:compAnimation():setAnimation("Attack")
            if(self.is_type2 == true) then
                self.missile_counter = self.missile_counter + dt

                if(self.missile_counter >= self.missile_cooldown) then
                     self.missile_counter = self.missile_counter - self.missile_cooldown

                    local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_world_pos = owner_body2d_comp:getWorldPosition()

                    local diff_owner_to_planet = EngineMath.minusVector(planet_pos, owner_world_pos)
                    local dir_owner_to_planet = EngineMath.unitVector(diff_owner_to_planet)

                    local missile_speed = 250.0
                    AudioManager.getInstance():playSoundEffect("Missile", 20)
                    owner_entity:compSpaceShip():shoot_new_projectile_guided(entity_manager, 
                        "UfoMissile", owner_world_pos, dir_owner_to_planet, missile_speed, self.planet_entity)
                                            
                end
               

            end
              
            --self.shoot_missile(self, owner_entity, entity_manager, grid_map, dt)
            if(self.elapsed_time >= self.time_shoot_projectile) then
                self.elapsed_time = self.elapsed_time - self.time_shoot_projectile

                local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
                local owner_body2d_comp = owner_entity:compBody2d()
                local owner_world_pos = owner_body2d_comp:getWorldPosition()

                local diff_owner_to_planet = EngineMath.minusVector(planet_pos, owner_world_pos)
                local dir_owner_to_planet = EngineMath.unitVector(diff_owner_to_planet)

                local ufo_laser_speed = 500.0

                if(self.is_type2 ~= true) then
                    AudioManager.getInstance():playSoundEffect("Laser_Enemy", 20)
                    owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, dir_owner_to_planet, ufo_laser_speed)
                else
                    local angle_dir_diff = 15

                    local main_dir_angle = EngineMath.vectorToDegree(dir_owner_to_planet)

                    local plus_angle = EngineMath.plusDegree(main_dir_angle, angle_dir_diff)
                    local minus_angle = EngineMath.minusDegree(main_dir_angle, angle_dir_diff)

                    local plus_dir = EngineMath.degreeToVector(plus_angle)
                    local minus_dir = EngineMath.degreeToVector(minus_angle)

                     AudioManager.getInstance():playSoundEffect("Laser_Enemy", 20)

                    owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, dir_owner_to_planet, ufo_laser_speed)
                    owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, plus_dir, ufo_laser_speed)
                    owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, minus_dir, ufo_laser_speed)
                end

                --self.shoot_projectiles(self, owner_entity, entity_maanger, grid_map, dt)
                --[[local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
                local owner_body2d_comp = owner_entity:compBody2d()
                local owner_world_pos = owner_body2d_comp:getWorldPosition()

                local diff_owner_to_planet = EngineMath.minusVector(planet_pos, owner_world_pos)
                local dir_owner_to_planet = EngineMath.unitVector(diff_owner_to_planet)

                local ufo_laser_speed = 500.0

                owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                    "UfoLaser", owner_world_pos, dir_owner_to_planet, ufo_laser_speed)]]
            end 
            
           

            
		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
        end,

        shoot_projectiles = function(self, owner_entity, entity_manager, grid_map, dt)
            local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
            local owner_body2d_comp = owner_entity:compBody2d()
            local owner_world_pos = owner_body2d_comp:getWorldPosition()

            local diff_owner_to_planet = EngineMath.minusVector(planet_pos, owner_world_pos)
            local dir_owner_to_planet = EngineMath.unitVector(diff_owner_to_planet)

            local ufo_laser_speed = 500.0

            if(self.is_type2 ~= true) then
                print("yo")
                owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                    "UfoLaser", owner_world_pos, dir_owner_to_planet, ufo_laser_speed)
            else
                local angle_dir_diff = 25

                local main_dir_angle = EngineMath.vectorToDegree(dir_owner_to_planet)

                local plus_angle = EngineMath.plusDegree(main_dir_angle, angle_dir_diff)
                local minus_angle = EngineMath.minusDegree(main_dir_angle, angle_dir_diff)

                local plus_dir = EngineMath.degreeToVector(plus_angle)
                local minus_dir = EngineMath.degreeToVector(minus_angle)

                owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                    "UfoLaser", owner_world_pos, dir_owner_to_planet, ufo_laser_speed)
                owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                    "UfoLaser", owner_world_pos, plus_dir, ufo_laser_speed)
                owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                    "UfoLaser", owner_world_pos, minus_dir, ufo_laser_speed)
            end

        end,

        shoot_missile = function(self, owner_entity, entity_manager, grid_map, dt)
            if(self.is_type2 ~= true) then
                return
            end

            self.missile_counter = self.missile_counter + dt

            if(self.missile_counter < self.missile_cooldown) then
                return
            end
            self.missile_counter = self.missile_counter - self.missile_cooldown

            local planet_pos = self.planet_entity:compBody2d():getWorldPosition()
            local owner_body2d_comp = owner_entity:compBody2d()
            local owner_world_pos = owner_body2d_comp:getWorldPosition()

            local diff_owner_to_planet = EngineMath.minusVector(planet_pos, owner_world_pos)
            local dir_owner_to_planet = EngineMath.unitVector(diff_owner_to_planet)

            local missile_speed = 350.0

            owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                "UfoMissile", owner_world_pos, dir_owner_to_planet, missile_speed)
        end,
        
        planet_entity = nil,
        is_type2 = false,
        elapsed_time = 0.0,
        time_shoot_projectile = 2.0,
        missile_counter = 0.0,
        missile_cooldown = 12.0,
                
        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                   
                    
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_world_pos = owner_body2d_comp:getWorldPosition()

                    

                    return false			   
                end,
				callTargetState = function(owner_entity)
                    local aiScriptComp = owner_entity:compScriptAI()
                    --aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlayerAIStates.ChasePlayerState))
                end

			}

        }

    },   
  

    initialStateFunc = function(owner_entity)
		aiScriptComp = owner_entity:compScriptAI()
		aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlanetAIStates.ChasePlanetState))
	end

}



dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")



local dist_to_shoot_projectiles = 550.0

AttackPlayerAIStates = {
  
    ChasePlayerState = 
    {
        name = "ChasePlayerState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            
            if (self.player_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex(Constant.Player_Layer), {},
                 
                    function(entity, interactEnt, luaData)
                        if(Utility.isBothIndexRelated(interactEnt:compCategory():getCategory(), Category.Player)) then
                            self.player_entity = interactEnt
                        end
                    end
                    )
            end

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            owner_auto_path_comp:clearAutomaticPath()

            

		end,

	    update = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAnimation():setAnimation("Idle")
            if(self.player_entity == nil) then
                return
            end
            local owner_auto_path_comp = owner_entity:compAutomaticPath()
            if(self.player_entity:compPlayer():is_player_available() ~= true) then
                owner_auto_path_comp:clearAutomaticPath()
                return
            end        

            

            local player_world_pos = 
               self.player_entity:compBody2d():getWorldPosition()

            self.elapsed_time = self.elapsed_time + dt
            if(self.elapsed_time >= self.time_refresh_path_list) then
                self.elapsed_time = self.elapsed_time - self.time_refresh_path_list
                owner_auto_path_comp:setDestToCalculate(player_world_pos.x, player_world_pos.y)
            end
            
		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
        end,

        player_entity = nil,
        min_safe_dist_to_player = 400.0,
        base_speed = 300.0,
        elapsed_time = 0.0,
        time_refresh_path_list = 0.4,

        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    if(self.player_entity:compPlayer():is_player_available() ~= true) then
                        return
                    end

                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                    local player_world_pos = 
                        self.player_entity:compBody2d():getWorldPosition()

                    local owner_world_pos = owner_body2d_comp:getWorldPosition()
                    local diff_player_to_owner = EngineMath.minusVector(player_world_pos, owner_world_pos)
            
                    local dist_player_to_owner = EngineMath.vectorLength(diff_player_to_owner)

                    if(dist_player_to_owner <= self.min_safe_dist_to_player) then              
                        return true
                    end

                    return false                 
                    
                end,			   

				callTargetState = function(owner_entity)
                    local aiScriptComp = owner_entity:compScriptAI()
                    aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlayerAIStates.AttackPlayerState))
                end

			}

        }

    },
    AttackPlayerState = 
    {
        name = "AttackPlayerState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            self.player_entity = nil
            self.is_type2 = owner_entity:compSpaceShip():has_projectile("UfoMissile")
            
            
            if (self.player_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex(Constant.Player_Layer), {},
                 
                    function(entity, interactEnt, luaData)
                        if(Utility.isBothIndexRelated(interactEnt:compCategory():getCategory(), Category.Player)) then
                            self.player_entity = interactEnt
                        end
                    end
                    )
            end

            self.last_recorded_player_pos_x = 0.0
            self.last_recorded_player_pos_y = 0.0

            local owner_body2d_comp = owner_entity:compBody2d()

            self.ori_base_speed = owner_body2d_comp:getBaseSpeed()
            owner_body2d_comp:setBaseSpeed(self.base_speed)

            owner_body2d_comp:setVelocity(0.0, 0.0, false)

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            owner_auto_path_comp:clearAutomaticPath()

            --self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)

		end,

	    update = function(self, owner_entity, entity_manager, grid_map, dt)
            --[[if(self.has_called_optimal_func ~= true) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
                print("1")
                return
            end]]
            owner_entity:compAnimation():setAnimation("Attack")
            if(self.player_entity == nil) then
                return
            end
            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            if(self.player_entity:compPlayer():is_player_available() ~= true) then
                owner_auto_path_comp:clearAutomaticPath()
                return
            end

            local owner_body2d_comp = owner_entity:compBody2d()
            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            local player_world_pos = 
                self.player_entity:compBody2d():getWorldPosition()

            --[[local recorded_player_pos = sfVector2f(self.last_recorded_player_pos_x, 
                self.last_recorded_player_pos_y)
            
            local diff_to_last_player_pos = 
                EngineMath.minusVector(player_world_pos, recorded_player_pos)

            local dist_to_last_player_pos = 
                EngineMath.vectorLength(diff_to_last_player_pos)

            if(dist_to_last_player_pos > self.dist_for_refresh_optimal_pos) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
                print("2")
                return
            end]]
            
            local owner_world_pos = owner_body2d_comp:getWorldPosition()
            local diff_player_to_owner = EngineMath.minusVector(player_world_pos, owner_world_pos)
            
            local dist_player_to_owner = EngineMath.vectorLength(diff_player_to_owner)

            if(owner_auto_path_comp:isAutomaticPathEmpty()) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
               
            end


            if(self.is_type2 == true) then
                local space_ship_comp = owner_entity:compSpaceShip()
                self.missile_counter = self.missile_counter + dt

                if(space_ship_comp:can_shoot_missile()) then
                    self.missile_counter = self.missile_counter - self.missile_cooldown
                    space_ship_comp:reset_missile_allowance()

                    local player_pos = self.player_entity:compBody2d():getWorldPosition()
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_world_pos = owner_body2d_comp:getWorldPosition()

                    local diff_owner_to_player = EngineMath.minusVector(player_pos, owner_world_pos)
                    local dir_owner_to_player = EngineMath.unitVector(diff_owner_to_player)

                    local missile_speed = 350.0

                    AudioManager.getInstance():playSoundEffect("Missile", 20)
                    space_ship_comp:shoot_new_projectile_guided(entity_manager, 
                        "UfoMissile", owner_world_pos, dir_owner_to_player, missile_speed, self.player_entity)
                                            
                end
               

            end
            self.elapsed_time = self.elapsed_time + dt
            --self.shoot_missile(self, owner_entity, entity_manager, grid_map, dt)
            if(self.elapsed_time >= self.time_shoot_projectile) then
            
                self.elapsed_time = self.elapsed_time - self.time_shoot_projectile

                local player_pos = self.player_entity:compBody2d():getWorldPosition()
                local owner_body2d_comp = owner_entity:compBody2d()
                local owner_world_pos = owner_body2d_comp:getWorldPosition()

                local diff_owner_to_player = EngineMath.minusVector(player_pos, owner_world_pos)
                local dir_owner_to_player = EngineMath.unitVector(diff_owner_to_player)

                local ufo_laser_speed = 500.0

                if(self.is_type2 ~= true) then
                     AudioManager.getInstance():playSoundEffect("Laser_Enemy", 20)
                    owner_entity:compSpaceShip():shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, dir_owner_to_player, ufo_laser_speed)
                else
                    local angle_dir_diff = 15

                    local main_dir_angle = EngineMath.vectorToDegree(dir_owner_to_player)

                    local plus_angle = EngineMath.plusDegree(main_dir_angle, angle_dir_diff)
                    local minus_angle = EngineMath.minusDegree(main_dir_angle, angle_dir_diff)

                    local plus_dir = EngineMath.degreeToVector(plus_angle)
                    local minus_dir = EngineMath.degreeToVector(minus_angle)

                    local space_ship_comp = owner_entity:compSpaceShip()

                    AudioManager.getInstance():playSoundEffect("Laser_Enemy", 20)
                    space_ship_comp:shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, dir_owner_to_player, ufo_laser_speed)
                    space_ship_comp:shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, plus_dir, ufo_laser_speed)
                    space_ship_comp:shoot_new_projectile(entity_manager, 
                        "UfoLaser", owner_world_pos, minus_dir, ufo_laser_speed)
                end
                                
            end 

            
		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
            local owner_body2d_comp = owner_entity:compBody2d()
                   
            owner_body2d_comp:setBaseSpeed(self.ori_base_speed)
        end,

        find_optimal_pos = function(self, owner_entity, entity_manager, grid_map)
            if(self.player_entity == nil) then
                return
            end
            
            self.has_called_optimal_func = true
            local found_optimal = false

            local owner_body2d_comp = owner_entity:compBody2d()

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            local owner_world_pos = owner_body2d_comp:getWorldPosition()


            local player_world_pos = 
                self.player_entity:compBody2d():getWorldPosition()

            repeat
                
                local random_degree = EngineUtil.randomRange(0, 360)

                local random_dir = EngineMath.degreeToVector(random_degree)

                local random_radius_deviation = EngineUtil.randomRange(
                    self.min_pos_radius_deviation, self.max_pos_radius_deviation)

                local random_pos = EngineMath.multiplyVectorByFloat(random_dir, random_radius_deviation)

                random_pos = EngineMath.plusVector(random_pos, player_world_pos)
                
                local random_grid_node = grid_map:toGrid(random_pos.x, random_pos.y)

                if(grid_map:is_entity_not_obstacled_on_node(owner_entity, random_grid_node)) then
                    self.last_recorded_player_pos_x = player_world_pos.x;
                    self.last_recorded_player_pos_y = player_world_pos.y;

                    owner_auto_path_comp:setDestToCalculate(
                        random_grid_node.mGridPos.x, random_grid_node.mGridPos.y)

                    found_optimal = true
                    
                end


            until(found_optimal)
        end,

        player_entity = nil,
        has_called_optimal_func = false,
        --dist_to_shoot_projectiles = 350.0,
        min_pos_radius_deviation = 150.0,
        max_pos_radius_deviation = 250.0,
        dist_for_refresh_optimal_pos = 250.0,
        last_recorded_player_pos_x = 0.0,
        last_recorded_player_pos_y = 0.0,

        ori_base_speed = 0.0,
        base_speed = 150.0,
        is_type2 = false,
        elapsed_time = 0.0,
        time_shoot_projectile = 2.0,
        missile_counter = 0.0,
        missile_cooldown = 10.0,
        
        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    if(self.player_entity:compPlayer():is_player_available() ~= true) then
                        return false
                    end

                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                    
                    
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_world_pos = owner_body2d_comp:getWorldPosition()

                    local player_world_pos = 
                        self.player_entity:compBody2d():getWorldPosition()


                    local diff_player_to_owner = EngineMath.minusVector(player_world_pos, owner_world_pos)
            
                    local dist_player_to_owner = EngineMath.vectorLength(diff_player_to_owner)

                    if(dist_player_to_owner > dist_to_shoot_projectiles) then              
                        return true
                    end

                    return false			   
                end,
				callTargetState = function(owner_entity)
                    local aiScriptComp = owner_entity:compScriptAI()
                    aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlayerAIStates.ChasePlayerState))
                end

			}

        }

    },   
  

    initialStateFunc = function(owner_entity)
		aiScriptComp = owner_entity:compScriptAI()
		aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlayerAIStates.ChasePlayerState))
	end

}



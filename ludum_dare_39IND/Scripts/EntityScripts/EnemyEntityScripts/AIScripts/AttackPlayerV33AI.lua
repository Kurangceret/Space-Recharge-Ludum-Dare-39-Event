dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")



local dist_to_shoot_projectiles = 350.0

AttackPlayerAIStates = {
  
    AttackPlayerState = 
    {
        name = "AttackPlayerState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            
            if (self.player_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex("player_layer"), {},
                 
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
            
            
		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
        end,

        player_entity = nil,
        --m_dist_to_shoot_projectiles = 400.0,


        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                    local player_world_pos = 
                        self.player_entity:compBody2d():getWorldPosition()

                    local owner_world_pos = owner_body2d_comp:getWorldPosition()
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
    ChasePlayerState = 
    {
        name = "ChasePlayerState",
		enter = function(self, owner_entity, entity_manager, grid_map, dt)
            self.player_entity = nil

            if (self.player_entity == nil) then
                entity_manager:pushInteractionFuncData(
                    owner_entity, true, entity_manager:layerToZIndex("player_layer"), {},
                 
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

            owner_body2d_comp:setVelocity(0.0, 0.0, false)

            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            owner_auto_path_comp:clearAutomaticPath()

            --self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)

		end,

	    update = function(self, owner_entity, entity_manager, grid_map, dt)
            if(self.has_called_optimal_func ~= true) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
                print("1")
                return
            end

            local owner_body2d_comp = owner_entity:compBody2d()
            local owner_auto_path_comp = owner_entity:compAutomaticPath()

            local player_world_pos = 
                self.player_entity:compBody2d():getWorldPosition()

            local recorded_player_pos = sfVector2f(self.last_recorded_player_pos_x, 
                self.last_recorded_player_pos_y)
            
            local diff_to_last_player_pos = 
                EngineMath.minusVector(player_world_pos, recorded_player_pos)

            local dist_to_last_player_pos = 
                EngineMath.vectorLength(diff_to_last_player_pos)

            if(dist_to_last_player_pos > self.dist_for_refresh_optimal_pos) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
                print("2")
                return
            end
            
            local owner_world_pos = owner_body2d_comp:getWorldPosition()
            local diff_player_to_owner = EngineMath.minusVector(player_world_pos, owner_world_pos)
            
            local dist_player_to_owner = EngineMath.vectorLength(diff_player_to_owner)

            if(dist_player_to_owner > dist_to_shoot_projectiles and
                (owner_auto_path_comp:isAutomaticPathEmpty())
                ) then
                self.find_optimal_pos(self, owner_entity, entity_manager, grid_map)
                print("3")
            end

            
		end,
		quit = function(self, owner_entity, entity_manager, grid_map, dt)
            owner_entity:compAutomaticPath():clearAutomaticPath()
        end,

        find_optimal_pos = function(self, owner_entity, entity_manager, grid_map)
            if(self.player_entity == nil) then
                return
            end
            print("find_optimal")
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
        
        
        Transitions = 
        {
			{
				isTriggered = function(self, owner_entity)
                    local owner_auto_path_comp = owner_entity:compAutomaticPath()

                    if(owner_auto_path_comp:isUncalculatedPosValid() or 
                        owner_auto_path_comp:isAutomaticPathEmpty() ~= true) then
                        return false
                    end
                    
                    local owner_body2d_comp = owner_entity:compBody2d()
                    local owner_world_pos = owner_body2d_comp:getWorldPosition()

                    local player_world_pos = 
                        self.player_entity:compBody2d():getWorldPosition()


                    local diff_player_to_owner = EngineMath.minusVector(player_world_pos, owner_world_pos)
            
                    local dist_player_to_owner = EngineMath.vectorLength(diff_player_to_owner)

                    if(dist_player_to_owner <= dist_to_shoot_projectiles) then              
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
  

    initialStateFunc = function(owner_entity)
		aiScriptComp = owner_entity:compScriptAI()
		aiScriptComp:setCurState(Utility.deepTableCopy(AttackPlayerAIStates.ChasePlayerState))
	end

}



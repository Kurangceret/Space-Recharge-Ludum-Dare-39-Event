--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")
dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")

local box_width =  70.0
local box_height = 12.0

local texture_width = 70
local texture_height = 12

local projectile_damage = 12.0
local projectile_drain = 8.0

local Entity = {
	initializeData = {
		SpriteComponent = {
			textureLocation = Constant.ProjectilesTextureDir .. "ufo_missile.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
			drawFixtures = false
		},
	    
		CategoryComponent = {
			category = Category.UfoMissile
      	},

        HealthComponent = {
			currentHealth = 12,
			maxHealth = 12,

		},
		DestroyableComponent = {
			isDestroyed = function(thisEntity)
				return thisEntity:compHealth():hasZeroHealth()
			end,
			isRemoveable = function(thisEntity)
				return thisEntity:compDestroyable():isDestroyed()
			end
		},

        ScriptUpdateComponent = {
			updateFunc = function(entityManager, thisEntity, dt, memberVars)
				if(memberVars.tracked_entity == nil) then
                    return
                end
                memberVars.elapsed_time = memberVars.elapsed_time + dt
                if(memberVars.has_idle ~= true) then
                    if(memberVars.elapsed_time >= memberVars.idle_time) then
                        memberVars.has_idle = true
                        memberVars.elapsed_time = memberVars.elapsed_time - memberVars.idle_time
                    else
                        return
                    end
                    
                end

                if(memberVars.has_tracked ~= true) then
                   

                    local tracked_world_pos = memberVars.tracked_entity:compBody2d():getWorldPosition()
                    local owner_body2d_comp = thisEntity:compBody2d()

                    local owner_world_pos = owner_body2d_comp:getWorldPosition()
                
                    local diff_owner_to_tracked = EngineMath.minusVector(tracked_world_pos, owner_world_pos)
                    local dir_owner_to_tracked = EngineMath.unitVector(diff_owner_to_tracked)

                
                    local approach_dt = memberVars.approach_rate * dt

                    owner_body2d_comp:setBaseSpeed(memberVars.projectile_speed)
                    local owner_velo = owner_body2d_comp:getLinearVelocity()
                    if(owner_velo.x == 0 and owner_velo.y == 0) then
                        owner_velo = owner_body2d_comp:getFacingDir()
                        owner_velo = EngineMath.multiplyVectorByFloat(owner_velo, owner_body2d_comp:getBaseSpeed())
                    end
                    

                    local new_velo = EngineMath.plusVector(owner_velo, 
                        EngineMath.multiplyVectorByFloat(dir_owner_to_tracked, approach_dt))
                
                    local new_dir = EngineMath.unitVector(new_velo)

                    owner_body2d_comp:setVelocity(new_dir.x, new_dir.y, true)

                     if(memberVars.elapsed_time >= memberVars.max_track_time) then
                        memberVars.has_tracked = true
                       
                    end
                end

			end,
			memberVariables  = 
            {
                approach_rate = 600.0,
                tracked_entity = nil,
                idle_time = 2.5,
                elapsed_time = 0.0,
                max_track_time = 5.0,

                projectile_speed = 400.0,
                has_idle = false,
                has_tracked = false
            
            }
		},

		Body2dComponent = {
			bodyType = "dynamic",
			baseSpeed = 200.0,
			positionX = 0.0,
			positionY = 0.0,
			gravityScale = 0.0,
			mass = 1.0,
			isBullet = false,
			isBodyActive = true,
            rotateBasedOnVelo = true,
			fixtures = 
			{
				{
					name = "MainFixture",
					isMainFixture = true,
					isActive = true,
					density = 0.0,
					friction = 0.0,
					restitution = 0,
					shape = {
						shapeType = "Box",
						width = box_width,
						height = box_height,
						originX = 0,
						originY = 0
					},
					category = Category.UfoMissile,
					categoryToBeCollided = {Category.NormalBox2DTile},
					categoryToBeCollisionSolved = {},
					beginContactFunction = function(thisEntity, collidedEntity, entityManager,
						thisFixtureLogic, collidedFixtureLogic)

                        if (thisEntity:compDestroyable():isDestroyed()) then
                            return
                        end
                        thisEntity:compHealth():doDirectDamage(thisEntity, 12)
                        
                        local collidedHealthComp = collidedEntity:compHealth()
                        
                        local player_comp = collidedEntity:compPlayer()
                        if(player_comp ~= nil) then
                            player_comp:increase_drain_power(projectile_drain)
                        end

                        if(collidedHealthComp ~= nil) then
                            collidedHealthComp:doDirectDamage(thisEntity, projectile_damage)
                        end 
						
					end
				}
			}
		}
	}
}

return Entity

--endregion

--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")
dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")

local box_width = 50.0
local box_height = 24.0

local texture_width = 63
local texture_height = 36

local projectile_damage = 8.0

local Entity = {
	initializeData = {
		SpriteComponent = {
			textureLocation = Constant.ProjectilesTextureDir .. "spaceship_color_laser.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
			drawFixtures = false
		},
	    
		AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "SpaceShipColorLaserAnimation.lua"

        },

        HealthComponent = {
			currentHealth = 1,
			maxHealth = 1,

		},
		DestroyableComponent = {
			isDestroyed = function(thisEntity)
				return thisEntity:compHealth():hasZeroHealth()
			end,
			isRemoveable = function(thisEntity)
				return (thisEntity:compDestroyable():isDestroyed() 
                    and thisEntity:compAnimation():has_dead_anim_played())
			end
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
					category = Category.Laser_Projectile,
					categoryToBeCollided = {Category.NormalBox2DTile},
					categoryToBeCollisionSolved = {},
					beginContactFunction = function(thisEntity, collidedEntity, entityManager,
						thisFixtureLogic, collidedFixtureLogic)

                        if (thisEntity:compDestroyable():isDestroyed()) then
                            return
                        end
                        thisEntity:compHealth():doDirectDamage(thisEntity, 1)
                        
                        thisEntity:compAnimation():setAnimation("Destroyed")
                        local collidedHealthComp = collidedEntity:compHealth()
                        thisEntity:compBody2d():setVelocity(0.0, 0.0)  
                                                
                       
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

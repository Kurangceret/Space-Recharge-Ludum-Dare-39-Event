dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")
dofile(Constant.ScriptDir .. "UtilityFunctionScript.lua")
print(Constant.ScriptDir .. "CategoryScript.lua")

local boxWidth = 83.0
local boxHeight = 42.0

local Player = {
	initializeData = {
		PlayerComponent = 
		{
			max_power = 700.0,
            cur_power = 700.0,
            cur_drain_power_rate = 10,
            min_drain_power_rate = 10,
            max_drain_power_rate = 30
		},

		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "spaceship_100.png",
			textureRect = {0, 0, boxWidth, boxHeight},
			spriteWorldOrigin = {-boxWidth / 2.0, -boxHeight / 2.0},
            --[[textureRect = {0, 0, 168, 138},
			spriteWorldOrigin = {-50, -69},]]
			drawFixtures = false
		},
	    AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "PlayerAnimation.lua"

        },
    
		CategoryComponent = {
			category = Category.Player
      
		},

		RealTimeInputComponent = {


		},

        AutomaticPathComponent = {


        },
				
		ScriptUpdateComponent = {
			updateFunc = function(entityManager, thisEntity, dt, memberVars)
				
			end,
			memberVariables  = {}
		},

        SpaceShipComponent = 
        {
            projectiles_list = 
            {
                {
                    name = "SpaceShipLaser",
                    target_category = Category.Enemy 
                },
                {
                    name = "SpaceShipColorLaser",
                    target_category = Category.Enemy 
                }

            }

        },
		
        --[[ParticleEmitterComponent = 
        {
            emitters_list = 
            {
                {
                    particle_system_name = "Cartoon_Star",

                    emission_rate = 30.0,

                    --emitter life time
                    is_infinite_life_time = true,
                    emitter_life_time = 0.0,

                    --particle data
                    min_life_time = 1.0,
                    max_life_time = 2.0,


                    local_pos_x = 0.0,
                    local_pos_y = 30.0,
                    local_radius = 20.0,

                    particle_magnitude = 100.0,

                    particle_main_dir_x = 0.0,
                    particle_main_dir_y = 1.0,
                    dir_angle_deviation = 45.0,

                    min_angle_rotation = 0.0,
                    max_angle_rotation = 360.0
                    
                }


            }



        },]]
		
		Body2dComponent = {
			bodyType = "Dynamic",
			baseSpeed = 200.0,
			positionX = 0.0,
			positionY = 0.0,
			gravityScale = 1.0,
			mass = 1.0,
			isBullet = true,
            rotateBasedOnVelo = true,
            steering_turning_rate = 760.0,
			fixtures = 
			{
				
				
				{
					name = "MainFixture",
					isMainFixture = true,
					density = 0.0,
					friction = 0.0,
					restitution = 0.0,
					shape = 
					{
						shapeType = "box",
						width = boxWidth,
                        height = boxHeight,
						originX = 0,
						originY = 0
					},
					category = Category.Player,
					categoryToBeCollided = 
                    {
                        Category.NormalBox2DTile,
                        Category.Projectile,
                        Category.PlanetEnemy,
                        Category.PlanetEarth
                        
                    },
					categoryToBeCollisionSolved = 
                    {
                        Category.NormalBox2DTile,
                        Category.PlanetEnemy
                        
                    },
					beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)
						
					end
				}
				

			}
		}
				
      
      
	},
    reactToPrintEvent = function(thisEntity, printEvent)
    
		
    
	end,
  
	initializeFunction = function(self, createdEntity)
    
		--[[eventManager = EventManager.getInstance()
		eventManager:addLuaListener(createdEntity, "PrintEvent", self.reactToPrintEvent)]]
		
	end
  
    
}

return Player


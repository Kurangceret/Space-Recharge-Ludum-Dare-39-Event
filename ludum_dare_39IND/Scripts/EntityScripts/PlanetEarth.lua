dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")


local box_width = 600.0
local box_height = 600.0

local texture_width = 735.0
local texture_height = 735.0

local Entity = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "earth_500.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
            drawFixtures = false
		},
    
		CategoryComponent = {
			category = Category.PlanetEarth
      
		},
        
        HealthComponent = {
			currentHealth = 800,
			maxHealth = 800,

		},
        --[[ObstacleComponent = {


        },]]
		Body2dComponent = 
		{
			bodyType = "static",
			baseSpeed = 100.0,
			positionX = 0.0,
			positionY = 0.0,
			gravityScale = 1.0,
			fixtures = 
			{
				{
					isMainFixture = true,
					density = 0.0,
					friction = 1.0,
					restitution = 0.0,
					shape = {
						shapeType = "box",
						width = box_width,
						height = box_height,
						originX = 0,
						originY = 0
						--[[shapeType = "circle",
						radius = 16.0,
						originX = 0.0,
						originY = 0.0]]
					},
					category = Category.PlanetEarth,
					categoryToBeCollided = 
                    {
                        
                    
                    
                    },
                    categoryToBeCollisionSolved = 
                    {
                    
                    
                    
                    },
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)


                    end
				},
                {
					isMainFixture = false,
					density = 0.0,
					friction = 1.0,
					restitution = 0.0,
					shape = {
					    shapeType = "circle",
						radius = 260.0,
						originX = 0.0,
						originY = 0.0
					},
					category = Category.PlanetEarth,
					categoryToBeCollided = 
                    {
                        Category.Projectile,
                        Category.Player
                    
                    
                    },
                    categoryToBeCollisionSolved = 
                    {
                    
                    
                    
                    },
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)

                        local collided_category = collidedEntity:compCategory():getCategory()

                        if(Utility.isBothIndexRelated(collided_category, Category.Player)) then
                            local player_comp = collidedEntity:compPlayer()
                            player_comp:set_is_player_in_charge_area(true)
                        end

                    end,
                    endContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)

                        local collided_category = collidedEntity:compCategory():getCategory()

                        if(Utility.isBothIndexRelated(collided_category, Category.Player)) then
                            local player_comp = collidedEntity:compPlayer()
                            player_comp:set_is_player_in_charge_area(false)
                        end

                    end
				}
			},
      
      
		}
    
    
	}
}
return Entity
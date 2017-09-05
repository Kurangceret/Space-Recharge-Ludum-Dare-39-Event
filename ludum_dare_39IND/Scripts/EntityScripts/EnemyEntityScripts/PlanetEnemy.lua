dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")


local box_width = 760.0
local box_height = 760.0

local texture_width = 891.0
local texture_height = 892.0

local Entity = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "planet_alien.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
            drawFixtures = false
		},
    
		CategoryComponent = {
			category = Category.PlanetEnemy
      
		},
        HealthComponent = {
			currentHealth = 1100,
			maxHealth = 1100,

		},
        PlanetSpawnComponent = {



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
						--[[shapeType = "box",
						width = box_width,
						height = box_height,
						originX = 0,
						originY = 0]]
						shapeType = "circle",
						radius = 380.0,
						originX = 0.0,
						originY = 0.0
					},
					category = Category.PlanetEnemy,
					categoryToBeCollided = 
                    {
                        Category.Player
                    
                    
                    },
                    categoryToBeCollisionSolved = 
                    {
                        Category.Player
                    
                    
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
						radius = 330.0,
						originX = 0.0,
						originY = 0.0
					},
					category = Category.PlanetEnemy,
					categoryToBeCollided = 
                    {
                        Category.Projectile
                    
                    
                    },
                    categoryToBeCollisionSolved = 
                    {
                    
                    
                    
                    },
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)


                    end
				}
			},
      
      
		}
    
    
	}
}
return Entity
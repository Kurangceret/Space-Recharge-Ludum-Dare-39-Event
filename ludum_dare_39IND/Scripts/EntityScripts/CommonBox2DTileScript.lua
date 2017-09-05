dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

local Tile = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "transparent_32_32.png",
			textureRect = {0, 0, 32, 32},
			spriteWorldOrigin = {-16.0, -16.0}
		},
    
		CategoryComponent = {
			category = Category.NormalBox2DTile
      
		},
        ObstacleComponent = {


        },
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
						width = 32.0,
						height = 32.0,
						originX = 0,
						originY = 0
						--[[shapeType = "circle",
						radius = 16.0,
						originX = 0.0,
						originY = 0.0]]
					},
					category = Category.NormalBox2DTile,
					categoryToBeCollided = {Category.SolidEntity},
                    categoryToBeCollisionSolved = {Category.SolidEntity},
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)


                    end
				}
			},
      
      
		}
    
    
	}
}
return Tile
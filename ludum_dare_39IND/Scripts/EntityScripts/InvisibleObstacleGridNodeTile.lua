dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

local Tile = {
	
	initializeData = 
	{
        
		
    
		CategoryComponent = {
			category = Category.NormalBox2DTile
      
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
                    isSensor = false,
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
					categoryToBeCollided = {Category.SolidEntity, Category.Projectile},
                    categoryToBeCollisionSolved = {Category.SolidEntity, Category.Projectile}
                    
				}
			},
      
      
		}
    
    
	}
}
return Tile
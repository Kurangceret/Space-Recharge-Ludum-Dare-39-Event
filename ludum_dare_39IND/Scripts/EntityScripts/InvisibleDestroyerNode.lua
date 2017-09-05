dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

local Tile = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "transparent_32_32.png",
			textureRect = {0, 0, 32, 32},
			spriteWorldOrigin = {-16.0, -16.0},
            drawFixtures = false
		},
    
		CategoryComponent = {
			category = Category.Destroyer
      
		},
        ObstacleComponent = {
            isDestroyer = true

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
					category = Category.Destroyer,
					categoryToBeCollided = {Category.Enemy},
                    categoryToBeCollisionSolved = {},
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)

						if(collidedEntity:isEntityToBeRemoved())then
                            return
                        end
						if(collidedEntity:compScriptAI():getCurAIStateName() == "RunawayState") then
                            collidedEntity:markEntityToBeRemoved()
                            event = Event.EntityCollidesEntityEvent()
						    event.mEntityA = thisEntity
						    event.mEntityB = collidedEntity
						    event.mFixtureLogicA = thisFixtureLogic
                            event.mFixtureLogicB = collidedFixtureLogic

                            EventManager.getInstance():queueEvent("EntityCollidesEntityEvent", event)
                        end
					end,
                    preSolveFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)
                        if(collidedEntity:isEntityToBeRemoved())then
                            return
                        end
						if(collidedEntity:compScriptAI():getCurAIStateName() == "RunawayState") then
                            collidedEntity:markEntityToBeRemoved()
                            event = Event.EntityCollidesEntityEvent()
						    event.mEntityA = thisEntity
						    event.mEntityB = collidedEntity
						    event.mFixtureLogicA = thisFixtureLogic
                            event.mFixtureLogicB = collidedFixtureLogic

                            EventManager.getInstance():queueEvent("EntityCollidesEntityEvent", event)
                        end
					end
				}
			},
      
      
		}
    
    
	}
}
return Tile
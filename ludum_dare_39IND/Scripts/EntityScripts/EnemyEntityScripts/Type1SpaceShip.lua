dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")


local boxWidth = 105.0
local boxHeight = 29.0


local Entity = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.EnemiesTextureDir .. "ufo_small.png",
			textureRect = {0, 0, boxWidth, boxHeight},
			spriteWorldOrigin = {-boxWidth / 2.0, -boxHeight / 2.0},
            drawFixtures = false
		},
        AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "Type1SpaceShipAnimation.lua"

        },
		CategoryComponent = {
			category = Category.Type1SpaceShip
      
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

        ScriptAIComponent = {
            AIScriptDir = Constant.EnemyAIScriptDir .. "AttackPlayerAI.lua",
            tableName = "AttackPlayer"

        },
        AutomaticPathComponent = {


        },
        --[[AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "SheepAnimationScript.lua"

        },]]
        SpaceShipComponent = 
        {
            projectiles_list = 
            {
                {
                    name = "UfoLaser",
                    target_category = Category.PlayerProperties
                }

            }

        },

        ScriptUpdateComponent = {
			updateFunc = function(entityManager, thisEntity, dt, memberVars)
			    --local body2dComp = thisEntity:compBody2d()
                --body2dComp:setAngleDegree(EngineMath.vectorToDegree(body2dComp:getLinearUnitVelocity()))	
			end,
			memberVariables  = {}
		},
        Body2dComponent = 
		{
			bodyType = "dynamic",
			baseSpeed = 200.0,
			positionX = 0.0,
			positionY = 0.0,
			gravityScale = 1.0,
            rotateBasedOnVelo = false,
            steering_turning_rate = 700.0,
			fixtures = 
			{
				{
					isMainFixture = true,
					density = 0.0,
					friction = 1.0,
					restitution = 0.0,
					shape = {
						shapeType = "box",
						width = boxWidth,
						height = boxHeight,
						originX = 0,
						originY = 0
						
					},
					category = Category.Type1SpaceShip,
					categoryToBeCollided = 
                    {
                        Category.NormalBox2DTile,
                        Category.Projectile
                        
                    },
                    categoryToBeCollisionSolved = {Category.NormalBox2DTile, Category.Player, Category.PlayerHouse},
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)

                      
                    end
				}
			},
      
      
		}
    
    
	}
}
return Entity
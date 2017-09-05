dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")


local box_width = 190.0
local box_height = 50.0


local texture_width = 300
local texture_height = 300

local Entity = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.EnemiesTextureDir .. "ufo_big.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
            drawFixtures = false
		},
        AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "Type2SpaceShipAnimation.lua"

        },
		CategoryComponent = {
			category = Category.Type2SpaceShip
      
		},
        ScriptAIComponent = {
            AIScriptDir = Constant.EnemyAIScriptDir .. "AttackPlayerAI.lua",
            tableName = "AttackPlayer"

        },
        HealthComponent = {
			currentHealth = 24,
			maxHealth = 24,

		},
		DestroyableComponent = {
			isDestroyed = function(thisEntity)
				return thisEntity:compHealth():hasZeroHealth()
			end,
			isRemoveable = function(thisEntity)
				return thisEntity:compDestroyable():isDestroyed()
			end
		},

        AutomaticPathComponent = {


        },
        --[[AnimationComponent = {
			animScriptDir = Constant.AnimationScriptDir .. "SheepAnimationScript.lua"

        },]]
        SpaceShipComponent = 
        {
            has_missile_ability = true,
            projectiles_list = 
            {
                {
                    name = "UfoLaser",
                    target_category = Category.PlayerProperties
                },
                {
                    name = "UfoMissile",
                    target_category = Category.PlayerProperties
                }

            }

        },

        ScriptUpdateComponent = {
			updateFunc = function(entityManager, thisEntity, dt, memberVars)
			    --local body2dComp = thisEntity:compBody2d()
                --body2dComp:setAngleDegree(EngineMath.vectorToDegree(body2dComp:getLinearUnitVelocity()))
                local space_ship_comp = thisEntity:compSpaceShip()

                if(space_ship_comp:can_shoot_missile() ~= true) then
                    memberVars.missile_counter = memberVars.missile_counter + dt
                    if(memberVars.missile_counter >= memberVars.missile_cooldown) then
                        memberVars.missile_counter = memberVars.missile_counter - memberVars.missile_cooldown
                        space_ship_comp:allow_missile_to_shoot()
                    end
                end
                	
			end,
			memberVariables  = 
            {
                missile_counter = 0.0,
                missile_cooldown = 12.0
            }
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
						width = box_width,
						height = box_height,
						originX = 0,
						originY = 0
						
					},
					category = Category.Type2SpaceShip,
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
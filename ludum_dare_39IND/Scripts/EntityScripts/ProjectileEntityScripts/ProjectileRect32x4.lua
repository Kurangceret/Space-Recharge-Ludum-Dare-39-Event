--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")
dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")

local boxWidth = 32.0
local boxHeight = 4.0
local projectileDamage = 5.0

local Entity = {
	initializeData = {
		SpriteComponent = {
			textureLocation = Constant.ProjectilesTextureDir .. "projectile_rect_32x4.png",
			textureRect = {0, 0, boxWidth, boxHeight},
			spriteWorldOrigin = {-boxWidth / 2.0, -boxHeight / 2.0},
			drawFixtures = true
		},
	    
		CategoryComponent = {
			category = Category.Projectile
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
				return thisEntity:compDestroyable():isDestroyed()
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
						width = boxWidth,
						height = boxHeight,
						originX = 0,
						originY = 0
					},
					category = Category.Projectile,
					categoryToBeCollided = {Category.NormalBox2DTile},
					categoryToBeCollisionSolved = {},
					beginContactFunction = function(thisEntity, collidedEntity, entityManager,
						thisFixtureLogic, collidedFixtureLogic)
                        
                        if (thisEntity:isEntityToBeRemoved()) then
                            
                            return
                        end

						local collidedFixtureCategory = collidedFixtureLogic:getCategory()
                        

                        if(Utility.isBothIndexRelated(collidedFixtureCategory, Category.Player)) then
                            thisEntity:markEntityToBeRemoved()
                            
                            if(collidedFixtureCategory == Category.PlayerMainBody) then
                                
                                collidedEntity:compPlayer():decreaseCurHealth(projectileDamage)
                                
                            end
                           
                        end
						
					end
				}
			}
		}
	}
}

return Entity

--endregion

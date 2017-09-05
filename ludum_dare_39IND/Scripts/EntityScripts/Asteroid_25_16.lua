dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

local texture_width = 25
local texture_height = 16

local box_width = 25
local box_height = 16

local obstacle_width = 32
local obstacle_height = 32

local collision_damage = 5.0


local Tile = {
	
	initializeData = 
	{
        
		SpriteComponent = {
			textureLocation = Constant.TextureDir .. "asteroid_25_16.png",
			textureRect = {0, 0, texture_width, texture_height},
			spriteWorldOrigin = {-texture_width / 2.0, -texture_height / 2.0},
            drawFixtures = false
		},
    
		CategoryComponent = {
			category = Category.NormalBox2DTile
      
		},
        --[[ObstacleComponent = {


        },]]

        ScriptUpdateComponent = {
			updateFunc = function(entityManager, thisEntity, dt, memberVars)
				if(memberVars.has_select_identifier ~= true) then
                    memberVars.has_select_identifier = true

                    memberVars.rotate_speed_per_sec = EngineUtil.randomRange(memberVars.min_rotate_speed_per_sec, 
                        memberVars.max_rotate_speed_per_sec)
                    memberVars.rotate_identifier = EngineUtil.randomRange(0, 2)
                    
                end

                local degree_inc = memberVars.rotate_speed_per_sec * dt
                local owner_body2d_comp = thisEntity:compBody2d()
                local cur_degree = owner_body2d_comp:getAngleDegree()

                local new_degree = 0
                if(memberVars.rotate_identifier == 1) then
                    new_degree = EngineMath.plusDegree(cur_degree, degree_inc)      
                elseif(memberVars.rotate_identifier == 0) then
                    new_degree = EngineMath.minusDegree(cur_degree, degree_inc)  
                end

                owner_body2d_comp:setAngleDegree(new_degree)
                
			end,
			memberVariables  = 
            {
                min_rotate_speed_per_sec = 30.0,
                max_rotate_speed_per_sec = 90.0,
                rotate_speed_per_sec = 90.0,
                rotate_identifier = 1,
                has_select_identifier = false
                
            }
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
                    isSensor = true,
					shape = {
						shapeType = "box",
						width = obstacle_width,
						height = obstacle_height,
						originX = 0,
						originY = 0
						
					},
					category = Category.NormalBox2DTile,
					categoryToBeCollided = {},
                    categoryToBeCollisionSolved = {},
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
						shapeType = "box",
						width = box_width,
						height = box_height,
						originX = 0,
						originY = 0
						
					},
					category = Category.NormalBox2DTile,
					categoryToBeCollided = {Category.SolidEntity, Category.Projectile},
                    categoryToBeCollisionSolved = {Category.SolidEntity, Category.Projectile},
                    beginContactFunction = function(thisEntity, collidedEntity, entityManager, 
						thisFixtureLogic, collidedFixtureLogic)
                                                
                        

                    end
				}
			},
      
      
		}
    
    
	}
}
return Tile
--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

frameSize = {200, 200}
spriteOrigin = {-frameSize[1] / 2.0, -frameSize[2] / 2.0}

--110, 24 travelling animation
--130, 26, travelling 

idle_frame_size = {150, 43}
idle_origin = {-110, -24}

dash_frame_size = {170, 47}
dash_origin = {-130, -26}

ability_frame_size = {426, 339}
ability_origin = {-ability_frame_size[1] / 2, -ability_frame_size[2] / 2}


Animations = 
{
    {
	    name = "Destroyed",
		textureLocation = Constant.TextureDir .. "spaceship_explode_200_200.png",
		origLists = 
        {
		    {
				frame = {frameSize[1] * 0, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 1, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 2, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 3, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 4, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 5, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 6, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 7, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 8, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 9, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 10, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 11, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 12, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 13, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 14, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 15, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {frameSize[1] * 16, frameSize[1] * 0, 0, 0},
				duration = 0.15
			},


		},
        is_dead_animation = false,
		wholeFrameSize = frameSize,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = false,
		isRepeating = false,
        transformOrigin = spriteOrigin

	},

    {
	    name = "Idle",
		textureLocation = Constant.TextureDir .. "spaceship_travelling_150_43.png",
		origLists = 
        {
		    {
				frame = {idle_frame_size[1] * 0, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {idle_frame_size[1] * 1, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {idle_frame_size[1] * 2, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {idle_frame_size[1] * 3, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {idle_frame_size[1] * 4, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {idle_frame_size[1] * 5, idle_frame_size[1] * 0, 0, 0},
				duration = 0.1
			}

		},
        is_dead_animation = false,
		wholeFrameSize = idle_frame_size,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = true,
		isRepeating = true,
        transformOrigin = idle_origin

	},

    {
	    name = "Dash",
		textureLocation = Constant.TextureDir .. "spaceship_dash_170_47.png",
		origLists = 
        {
		    {
				frame = {dash_frame_size[1] * 0, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 1, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 2, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 3, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 4, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 5, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			}

		},
        is_dead_animation = false,
		wholeFrameSize = dash_frame_size,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = true,
		isRepeating = true,
        transformOrigin = dash_origin

	},
    {
	    name = "Ability",
		textureLocation = Constant.TextureDir .. "sspaceship_2nd_ability_426_339.png",
		origLists = 
        {
            {
				frame = {dash_frame_size[1] * 0, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 1, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 2, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 3, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 4, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 5, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 6, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 7, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},
            {
				frame = {dash_frame_size[1] * 8, dash_frame_size[1] * 0, 0, 0},
				duration = 0.05
			},

        },
        is_dead_animation = false,
		wholeFrameSize = ability_frame_size,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = true,
		isRepeating = true,
        transformOrigin = ability_origin
    }
    
}


--endregion

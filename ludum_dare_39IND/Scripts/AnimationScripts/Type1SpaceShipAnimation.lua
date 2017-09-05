--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

frameSize = {180, 172}
spriteOrigin = {frameSize[1] / 2.0, frameSize[2] / 2.0}

idle_frame_size = {105, 29}
idle_sprite_origin = {-idle_frame_size[1] / 2.0, -idle_frame_size[2] / 2.0}

Animations = 
{
    {
	    name = "Destroyed",
		textureLocation = Constant.EnemiesTextureDir .. "space_ship_laser_explode_180_172.png",
		origLists = 
        {
		    {
				frame = {frameSize[1] * 0, frameSize[1] * 0, 0, 0},
				duration = 0.1
			},


		},
        is_dead_animation = true,
		wholeFrameSize = frameSize,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = false,
		isRepeating = false,
        transformOrigin = spriteOrigin

	},

    {
	    name = "Idle",
		textureLocation = Constant.EnemiesTextureDir .. "small_ufo_105_29.png",
		origLists = 
        {
		    {
				frame = {idle_frame_size[1] * 0, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {idle_frame_size[1] * 1, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {idle_frame_size[1] * 2, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},

		},
        is_dead_animation = false,
		wholeFrameSize = idle_frame_size,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = true,
		isRepeating = true,
        transformOrigin = idle_sprite_origin

	},

    {
	    name = "Attack",
		textureLocation = Constant.EnemiesTextureDir .. "small_ufo_105_29.png",
		origLists = 
        {
		    {
				frame = {idle_frame_size[1] * 3, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {idle_frame_size[1] * 4, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},
            {
				frame = {idle_frame_size[1] * 5, idle_frame_size[1] * 0, 0, 0},
				duration = 0.15
			},

		},
        is_dead_animation = false,
		wholeFrameSize = idle_frame_size,
		wholeDuration = 0.2,
		flipX = false,
		endToStart = true,
		isRepeating = true,
        transformOrigin = idle_sprite_origin

	}
    
}


--endregion

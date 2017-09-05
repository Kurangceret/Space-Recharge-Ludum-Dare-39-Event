--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(Constant.ScriptDir .. "CategoryScript.lua")

frameSize = {180, 172}
spriteOrigin = {-frameSize[1] / 2.0, -frameSize[2] / 2.0}


Animations = 
{
    {
	    name = "Destroyed",
		textureLocation = Constant.ProjectilesTextureDir .. "space_ship_laser_explode_180_172.png",
		origLists = 
        {
		    {
				frame = {frameSize[1] * 0, frameSize[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {frameSize[1] * 1, frameSize[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {frameSize[1] * 2, frameSize[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {frameSize[1] * 3, frameSize[1] * 0, 0, 0},
				duration = 0.1
			},
            {
				frame = {frameSize[1] * 4, frameSize[1] * 0, 0, 0},
				duration = 0.1
			}


		},
        is_dead_animation = true,
		wholeFrameSize = frameSize,
		wholeDuration = 1.0,
		flipX = false,
		endToStart = false,
		isRepeating = false,
        transformOrigin = spriteOrigin

	}
    
}


--endregion

--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")


MusicDirectoryData = {}
MusicDirectoryData.MainTheme = Constant.MusicDir .. "Main_Theme.ogg"

IteratedMusicDirectoryData = {}
local i = 1
for k, v in pairs(MusicDirectoryData) do
  IteratedMusicDirectoryData[i] = {k, v}
  i = i + 1
end

SoundEffectDirectoryData = {}
SoundEffectDirectoryData.Ability_2nd = Constant.SoundEffectDir .. "ability_2nd.ogg"
SoundEffectDirectoryData.Dash = Constant.SoundEffectDir .. "dash_1.ogg"
SoundEffectDirectoryData.Invasion = Constant.SoundEffectDir .. "invasion_alarm.ogg"
SoundEffectDirectoryData.Laser_Enemy = Constant.SoundEffectDir .. "laser_enemy.ogg"
SoundEffectDirectoryData.Laser_SpaceShip1 = Constant.SoundEffectDir .. "laser_spaceship.ogg"
SoundEffectDirectoryData.Laser_SpaceShip2 = Constant.SoundEffectDir .. "laser_spaceship_2.ogg"
SoundEffectDirectoryData.Missile = Constant.SoundEffectDir .. "missile.ogg"
SoundEffectDirectoryData.Recharge = Constant.SoundEffectDir .. "recharge.ogg"
SoundEffectDirectoryData.SpaceShip_Explode = Constant.SoundEffectDir .. "spaceship_explosion.ogg"

--SoundEffectDirectoryData.Pickup_Coin = Constant.SoundEffectDir ..  "Pickup_Coin.wav"


IteratedSoundEffectDirectoryData = {}
i = 1
for k, v in pairs(SoundEffectDirectoryData) do
  IteratedSoundEffectDirectoryData[i] = {k, v}
  i = i + 1
end
--endregion

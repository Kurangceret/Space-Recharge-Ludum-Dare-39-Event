dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")

print(Constant.EntityScriptDir)
EntityScriptDirData = {}
--EntityScriptDirData.CommonTile = Constant.EntityScriptDir .. "CommonTileScript.lua"
--EntityScriptDirData.GrassTile = Constant.EntityScriptDir ..  "GrassTileScript.lua"
EntityScriptDirData.PlayerEntity = Constant.EntityScriptDir ..  "PlayerEntityScript.lua"
EntityScriptDirData.CommonBox2DTile = Constant.EntityScriptDir ..  "CommonBox2DTileScript.lua"


EntityScriptDirData.Type1SpaceShip = Constant.EnemyEntityScriptDir .. "Type1SpaceShip.lua"
EntityScriptDirData.Type2SpaceShip = Constant.EnemyEntityScriptDir .. "Type2SpaceShip.lua"


EntityScriptDirData.PlanetEnemy = Constant.EnemyEntityScriptDir .. "PlanetEnemy.lua"
EntityScriptDirData.PlanetEarth = Constant.EntityScriptDir .. "PlanetEarth.lua"


EntityScriptDirData.Asteroid_73_50 = Constant.EntityScriptDir .. "Asteroid_73_50.lua"
EntityScriptDirData.Asteroid_45_43 = Constant.EntityScriptDir .. "Asteroid_45_43.lua"
EntityScriptDirData.Asteroid_25_16 = Constant.EntityScriptDir .. "Asteroid_25_16.lua"

EntityScriptDirData.UfoLaser = Constant.ProjectileEntityScriptDir .. "UfoLaser.lua"
EntityScriptDirData.UfoMissile = Constant.ProjectileEntityScriptDir .. "UfoMissile.lua"
EntityScriptDirData.SpaceShipLaser = Constant.ProjectileEntityScriptDir .. "SpaceShipLaser.lua"
EntityScriptDirData.SpaceShipColorLaser = Constant.ProjectileEntityScriptDir .. "SpaceShipColorLaser.lua"


--EntityScriptDirData.InvisibleDestroyerNode = Constant.EntityScriptDir .. "InvisibleDestroyerNode.lua"

EntityScriptDirData.InvisibleObstacleGridNode = Constant.EntityScriptDir .. "InvisibleObstacleGridNodeTile.lua"

EntityScriptDirData.GrassTile = Constant.EntityScriptDir .. "GrassTileScript.lua"

IteratedEntityScriptDirData = {}
local i = 1
for k, v in pairs(EntityScriptDirData) do
  IteratedEntityScriptDirData[i] = {v, k}
  i = i + 1
end


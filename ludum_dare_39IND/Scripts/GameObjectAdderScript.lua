dofile(EngineConstant.ScriptDir .. "ConstantScript.lua")
dofile(EngineConstant.ScriptDir .. "GameObjectTypeScript.lua")
dofile(EngineConstant.ScriptDir .. "EntityScriptDirectoryData.lua")
dofile(EngineConstant.ScriptDir .. "UtilityFunctionScript.lua")

local GameObjectAdder = {}


GameObjectAdder.addCommonBox2DGameObject = function(self, entityManager, importedTileData, scriptDir)
    local luaData = {}
    luaData.finalPos = importedTileData.mWorldPos
	entityManager:queueEntity(importedTileData.mLayerName, scriptDir, "", true, luaData,
		function(createdEntity, luaData)
			createdEntity:compBody2d():setPosition(luaData.finalPos.x, luaData.finalPos.y)
		end)
end

GameObjectAdder.addSizeableSpriteableGameObject = function(self, entityManager, importedTileData, scriptDir)
    local luaData = {}
    luaData.finalPos = importedTileData.mWorldPos
    luaData.textureSprite = importedTileData.mTileTexture
    luaData.tileMap = importedTileData.mTiledMap
    luaData.tileSetId = importedTileData.mTileSetId
    luaData.tileId = importedTileData.mTileId

    if(importedTileData:isTexturesManagerInt()) then
        luaData.textureIntRect = importedTileData.mIntRect
    end

    entityManager:queueEntity(importedTileData.mLayerName, scriptDir, "", true, luaData,
		function(createdEntity, luaData)
            local spriteComp = createdEntity:compSprite()
			spriteComp:setTexture(luaData.textureSprite)
            spriteComp:setTextureRect(luaData.textureIntRect)
            createdEntity:compBody2d():setPosition(luaData.finalPos.x, luaData.finalPos.y)
		end)

end

GameObjectAdder.addInstanceableGameObject = function(self, entityManager, importedTileData, scriptDir)
    local luaData = {}
    luaData.finalPos = importedTileData.mWorldPos
    luaData.tileMap = importedTileData.mTiledMap
    luaData.layerName = importedTileData.mLayerName
    luaData.tileSetId = importedTileData.mTileSetId
    luaData.tileId = importedTileData.mTileId

    entityManager:queueEntity(importedTileData.mLayerName, scriptDir, "", true, luaData,
		function(createdEntity, luaData)
			createdEntity:compBody2d():setPosition(luaData.finalPos.x, luaData.finalPos.y)

            local objectData = luaData.tileMap:getObjectData(luaData.tileSetId, luaData.tileId, luaData.layerName)
            createdEntity:compInstance():callInstanceLuaFunc(createdEntity, objectData)
		end)

end



GameObjectAdder.checkGameObjectType = function(self, entityManager, importedTileData, engineObjInitializer)
	if (importedTileData.mGameObjectType == GameObjectType.Player) then
		self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.PlayerEntity)

	elseif (importedTileData.mGameObjectType == GameObjectType.CommonBox2DTile) then
		self:addSizeableSpriteableGameObject(entityManager, importedTileData, EntityScriptDirData.CommonBox2DTile)

    elseif (importedTileData.mGameObjectType == GameObjectType.Asteroid_73_50) then
		self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.Asteroid_73_50)

    elseif (importedTileData.mGameObjectType == GameObjectType.Asteroid_45_43) then
		self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.Asteroid_45_43)

    elseif (importedTileData.mGameObjectType == GameObjectType.Asteroid_25_16) then
		self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.Asteroid_25_16)
    
    elseif(importedTileData.mGameObjectType == GameObjectType.Type1SpaceShip) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.Type1SpaceShip)

     elseif(importedTileData.mGameObjectType == GameObjectType.Type2SpaceShip) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.Type2SpaceShip)

    elseif(importedTileData.mGameObjectType == GameObjectType.PlanetEarth) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.PlanetEarth)

    elseif(importedTileData.mGameObjectType == GameObjectType.PlanetEnemy) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.PlanetEnemy)

    elseif(importedTileData.mGameObjectType == GameObjectType.InvisibleObstacleGridNode) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.InvisibleObstacleGridNode)

   
    
    elseif(importedTileData.mGameObjectType == GameObjectType.InvisibleDestroyerNode) then
        self:addCommonBox2DGameObject(entityManager, importedTileData, EntityScriptDirData.InvisibleDestroyerNode)
    end
end


return GameObjectAdder
#include "ComponentArranger.h"
#include "../Entity/Entity.h"
#include "../Helper/Utility.h"
#include "../WindowState/GeneralData.h"
#include "../Helper/Constant.h"
#include "../Component/AllEntityComponentsHeader.h"
#include <Box2D\Dynamics\b2Body.h>
#include <Box2D\Dynamics\b2Fixture.h>
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include <Box2D\Collision\Shapes\b2EdgeShape.h>
#include <Box2D\Collision\Shapes\b2PolygonShape.h>


ComponentArranger::ComponentArranger(GeneralData* generalData, 
	luabridge::LuaRef entityNameToScriptDirMap,
	luabridge::LuaRef iteratedEntityNameToScriptDirMap,
	luabridge::LuaRef utilityTableFunc,
	lua_State* mLuaState)
:mTexturesStringManager(*generalData->getTexturesStringManager()),
mSystemManager(generalData->getSystemManager()),
mGeneralData(generalData),
mEntityNameToScriptDir(entityNameToScriptDirMap),
mLuaState(mLuaState),
mLuaUtilityTableFunc(utilityTableFunc)
{
	int i = 1;
	while (!iteratedEntityNameToScriptDirMap[i].isNil()){
		luabridge::LuaRef curTable = iteratedEntityNameToScriptDirMap[i];
		std::string scriptDir = curTable[1].cast<std::string>();
		std::string entityTableName = curTable[2].cast<std::string>();
		readOnlyScript(scriptDir);

		mEntityScriptDirToName.insert(std::make_pair(scriptDir, entityTableName));
		i++;
	}

}


ComponentArranger::~ComponentArranger()
{
}


std::string ComponentArranger::entScriptDirToEntName(const std::string& entityScriptDir) const
{
	auto iter = mEntityScriptDirToName.find(entityScriptDir);
	assert(iter != mEntityScriptDirToName.end());
	return iter->second;
}

luabridge::LuaRef ComponentArranger::getPremadeEntityTableData(const std::string& entityTableName)
{
	auto iter = mEntityInitialTableMap.find(mEntityNameToScriptDir[entityTableName]);
	assert(iter != mEntityInitialTableMap.end());
	return iter->second["initializeData"];
}

void ComponentArranger::readOnlyScript(const std::string& scriptDir)
{
	if (luaL_dofile(mLuaState, scriptDir.c_str()) != 0){
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		return;
	}
	luabridge::LuaRef initialTable = luabridge::LuaRef::fromStack(mLuaState, -1);
	mEntityInitialTableMap.insert(std::make_pair(scriptDir, initialTable));
}

void ComponentArranger::readFromLuaPremadeScript(std::shared_ptr<Entity>& entity,
	const std::string& entityName)
{
	assert(mEntityNameToScriptDir[entityName].isNil() == false);

	std::string scriptDir = mEntityNameToScriptDir[entityName].cast<std::string>();
	auto iterFindTable = mEntityInitialTableMap.find(scriptDir);
	assert(iterFindTable != mEntityInitialTableMap.end());

	readFromLuaScript(entity, iterFindTable->second);
}

void ComponentArranger::readFromLuaScriptName(std::shared_ptr<Entity>& entity, const std::string& scriptFilename,
	const std::string& tableName)
{
	if (luaL_dofile(mLuaState, scriptFilename.c_str()) != 0){
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		return;
	}

	luaL_openlibs(mLuaState);
	
	auto iterMap = mEntityInitialTableMap.find(scriptFilename);
	
	if (iterMap == mEntityInitialTableMap.end()){
		luabridge::LuaRef initialTable = luabridge::getGlobal(
			mLuaState, tableName.c_str());

		readFromLuaScript(entity, initialTable);
		mEntityInitialTableMap.insert(std::make_pair(scriptFilename, initialTable));
	}
	else{
		luabridge::LuaRef table = iterMap->second;
		readFromLuaScript(entity, table);
	}
}

void ComponentArranger::readFromLuaScriptReturn(std::shared_ptr<Entity>& entity, 
	const std::string& scriptFilename)
{
	
	//means we get the first return from the .lua file

	auto iterMap = mEntityInitialTableMap.find(scriptFilename);

	if (iterMap == mEntityInitialTableMap.end()){

		if (luaL_dofile(mLuaState, scriptFilename.c_str()) != 0){
			std::cout << lua_tostring(mLuaState, -1) << std::endl;
			return;
		}

		luabridge::LuaRef initialTable = luabridge::LuaRef::fromStack(mLuaState, -1);
		readFromLuaScript(entity, initialTable);
	}
	else{
		luabridge::LuaRef table = iterMap->second;
		readFromLuaScript(entity, table);
	}
}

void ComponentArranger::readFromLuaScript(std::shared_ptr<Entity>& entity, 
	luabridge::LuaRef& initialTable)
{
		
	if (initialTable.isNil())
		return;

	luabridge::LuaRef table = initialTable["initializeData"];
	
	try{
		if (!table["Body2dComponent"].isNil() && table["Body2dComponent"].isTable())
			readBody2dComponent(entity.get(), table["Body2dComponent"].cast<luabridge::LuaRef>());

		if (!table["SpriteComponent"].isNil() && table["SpriteComponent"].isTable())
			readSpriteComponent(entity.get(),
			table["SpriteComponent"].cast<luabridge::LuaRef>());
				

		if (!table["HealthComponent"].isNil() && table["HealthComponent"].isTable())
			readHealthComponent(entity.get(),
			table["HealthComponent"].cast<luabridge::LuaRef>());
				
		if (!table["AnimationComponent"].isNil() && table["AnimationComponent"].isTable())
			readAnimationComponent(entity.get(),
			table["AnimationComponent"].cast<luabridge::LuaRef>());

		if (!table["ScriptUpdateComponent"].isNil() && table["ScriptUpdateComponent"].isTable())
			readScriptUpdateComponent(entity.get(),
			table["ScriptUpdateComponent"].cast<luabridge::LuaRef>());

		if (!table["CategoryComponent"].isNil() && table["CategoryComponent"].isTable())
			readCategoryComponent(entity.get(),
			table["CategoryComponent"].cast<luabridge::LuaRef>());


		if (!table["TextDisplayComponent"].isNil() && table["TextDisplayComponent"].isTable())
			readTextDisplayComponent(entity.get(),
			table["TextDisplayComponent"].cast<luabridge::LuaRef>());
				
		if (!table["TimerComponent"].isNil() && table["TimerComponent"].isTable())
			readTimerComponent(entity.get(),
			table["TimerComponent"].cast<luabridge::LuaRef>());

		if (!table["DestroyableComponent"].isNil() && table["DestroyableComponent"].isTable())
			readDestroyableComponent(entity.get(),
			table["DestroyableComponent"].cast<luabridge::LuaRef>());

		if (!table["PlayerComponent"].isNil() && table["PlayerComponent"].isTable())
			readPlayerComponent(entity.get(),
			table["PlayerComponent"].cast<luabridge::LuaRef>());

		if (!table["AutomaticPathComponent"].isNil() && table["AutomaticPathComponent"].isTable())
			readAutomaticPathComponent(entity.get(),
			table["AutomaticPathComponent"].cast<luabridge::LuaRef>());
			
		
		if (!table["ObstacleComponent"].isNil() && table["ObstacleComponent"].isTable())
			readObstacleComponent(entity.get(), table["ObstacleComponent"].cast<luabridge::LuaRef>());

		if (!table["EntityParentComponent"].isNil() && table["EntityParentComponent"].isTable())
			arrangeChildEntityList(entity.get(),
			table["EntityParentComponent"].cast<luabridge::LuaRef>());

		if (!table["AutoBody2dMovementComponent"].isNil() && table["AutoBody2dMovementComponent"].isTable())
			readAutoBody2dMovementComponent(entity.get(), 
				table["AutoBody2dMovementComponent"].cast<luabridge::LuaRef>());

		if (!table["JumpComponent"].isNil() && table["JumpComponent"].isTable())
			readJumpComponent(entity.get(), table["JumpComponent"].cast<luabridge::LuaRef>());

		if (!table["RealTimeInputComponent"].isNil() && table["RealTimeInputComponent"].isTable())
			readRealTimeInputComponent(entity.get(), table["RealTimeInputComponent"].cast<luabridge::LuaRef>());

		if (!table["InstanceComponent"].isNil() && table["InstanceComponent"].isTable())
			readInstanceComponent(entity.get(), table["InstanceComponent"].cast<luabridge::LuaRef>());

		if (!table["ScriptAIComponent"].isNil() && table["ScriptAIComponent"].isTable())
			readScriptAIComponent(entity.get(), table["ScriptAIComponent"].cast<luabridge::LuaRef>());

		if (!table["ParticleEmitterComponent"].isNil() && table["ParticleEmitterComponent"].isTable())
			readParticleEmitterComponent(entity.get(), table["ParticleEmitterComponent"].cast<luabridge::LuaRef>());

		if (!table["SpaceShipComponent"].isNil() && table["SpaceShipComponent"].isTable())
			readSpaceShipComponent(entity.get(), table["SpaceShipComponent"].cast<luabridge::LuaRef>());

		if (!table["PlanetSpawnComponent"].isNil() && table["PlanetSpawnComponent"].isTable())
			readPlanetSpawnComponent(entity.get(), table["PlanetSpawnComponent"].cast<luabridge::LuaRef>());
		
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() <<"7"<< std::endl;
	}
	if (!initialTable["initializeFunction"].isNil() && initialTable["initializeFunction"].isFunction())
	{
		luabridge::LuaRef func = initialTable["initializeFunction"];
		try{
			func(initialTable, entity.get());
		}
		catch (luabridge::LuaException e){
			std::cout << e.what() <<"8"<< std::endl;
		}
	}

	mSystemManager->putEntityInSystems(entity);
}

void ComponentArranger::arrangeChildEntityList(Entity* entity,
	luabridge::LuaRef& table)
{
	EntityParentComponent* entityParentComp = entity->createComp<EntityParentComponent>();
	int index = 1;
	while (!table[index].isNil()){
		luabridge::LuaRef curIndex = table[index];
		std::string entityName = curIndex["entityName"].cast<std::string>();
		int posInd = 1;

		luabridge::LuaRef posListTable = curIndex["positionList"].cast<luabridge::LuaRef>();
		while (!posListTable[posInd].isNil()){
			std::shared_ptr<Entity> childEntity = mGeneralData->getEntityManager()->createEntity(entity->getZIndex());

			EntityChildrenComponent* childEntityParentComp = childEntity->createComp<EntityChildrenComponent>();
			childEntityParentComp->mParent = entity;

			this->readFromLuaPremadeScript(childEntity, entityName);

			
			entityParentComp->insertNewChild(childEntity.get());

			
			Body2dComponent* body2dComp = childEntity->nonCreateComp<Body2dComponent>();

			luabridge::LuaRef posTable = posListTable[posInd].cast<luabridge::LuaRef>();

			/*std::string posTypeStr = Utility::transformStrToLowerCase( posTable["posType"].cast<std::string>());

			if (posTypeStr == "absolute"){
				std::string rotationalTypeStr = Utility::transformStrToLowerCase(
					posTable["rotationalFromParentType"].cast<std::string>());
				AbsoluteChildPosType::AbsRotationalType rotationalType;
				if (rotationalTypeStr == "absolute")
					rotationalType = AbsoluteChildPosType::AbsToCurDir;
				else if (rotationalTypeStr == "offset")
					rotationalType = AbsoluteChildPosType::OffsetToCurDir;
				else if (rotationalTypeStr == "none")
					rotationalType = AbsoluteChildPosType::NoneToCurDir;

				float x = posTable["positionX"].cast<float>();
				float y = posTable["positionY"].cast<float>();

				std::unique_ptr<AbsoluteChildPosType> absoluteChildPosType(new AbsoluteChildPosType());
				absoluteChildPosType->mRotationalType = rotationalType;
				absoluteChildPosType->mAbsolutePos.x = x;
				absoluteChildPosType->mAbsolutePos.y = y;

				childEntityParentComp->mChildPosTypePtr.reset(absoluteChildPosType.release());

			}
			else if (posTypeStr == "relative"){
				std::unique_ptr<RelativeChildPosType> relativePosTypePtr(new RelativeChildPosType);
				relativePosTypePtr->mDegreeRelToParentDir = posTable["degreeFromParentFaceDir"].cast<float>();
				relativePosTypePtr->mDistToParentOrig.x = posTable["distanceFromParentOriginX"].cast<float>();
				relativePosTypePtr->mDistToParentOrig.y = posTable["distanceFromParentOriginY"].cast<float>();
				relativePosTypePtr->mOpossiteDist.x = posTable["oppositeDistX"].cast<float>();
				relativePosTypePtr->mOpossiteDist.y = posTable["oppositeDistY"].cast<float>();
				childEntityParentComp->mChildPosTypePtr.reset(relativePosTypePtr.release());
			}*/
			float x = posTable["positionX"].cast<float>();
			float y = posTable["positionY"].cast<float>();

			
			body2dComp->setPosition(x, y);
			
			posInd++;
		}
		index++;
	}

}

void ComponentArranger::readAutomaticPathComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	entity->createComp<AutomaticPathComponent>();
}

void ComponentArranger::readCategoryComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	CategoryComponent* categoryComp = entity->createComp<CategoryComponent>();
	if (!categoryComp && entity->hasComp<CategoryComponent>())
		categoryComp = entity->getComp<CategoryComponent>();
	if (!categoryComp) return;

	if (!table["category"].isNil())
		categoryComp->setCategory(table["category"].cast<std::size_t>());
}



void ComponentArranger::readSpriteComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	SpriteComponent* spriteComp = entity->createComp<SpriteComponent>();
	if (!spriteComp && entity->hasComp<SpriteComponent>())
		spriteComp = entity->getComp<SpriteComponent>();
	if (!spriteComp) return;

	if (!table["textureLocation"].isNil() && table["textureLocation"].isString()){
		/*std::cout << table["textureLocation"].cast<std::string>();
		mTexturesStringManager.loadTextures(table["textureLocation"].cast<std::string>());*/
		sf::Texture* texture = mTexturesStringManager.loadTextures(table["textureLocation"].cast<std::string>());
		if (texture){
			spriteComp->mSprite.setTexture(*texture);

			if (!table["textureRect"].isNil()){
				luabridge::LuaRef textureRectT = table["textureRect"].cast<luabridge::LuaRef>();
				spriteComp->mSprite.setTextureRect(sf::IntRect(
					textureRectT[1], textureRectT[2], textureRectT[3], textureRectT[4]));
			}
			if (!table["spriteWorldOrigin"].isNil()){
				luabridge::LuaRef spriteWorldOriginT = table["spriteWorldOrigin"].cast<luabridge::LuaRef>();
				spriteComp->mSpriteWorldOrigin = sf::Vector2f(spriteWorldOriginT[1].cast<float>()
					, spriteWorldOriginT[2].cast<float>());
			}
		}

		
	}

	if (!table["drawEntID"].isNil())
		spriteComp->mDrawEntName = table["drawEntID"].cast<bool>();
	if (!table["drawFixtures"].isNil())
		spriteComp->mDrawFixtures = table["drawFixtures"].cast<bool>();
	if (!table["drawNonActiveFixtures"].isNil())
		spriteComp->mDrawNonActiveFixtures = table["drawNonActiveFixtures"].cast<bool>();
}



void ComponentArranger::readHealthComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	HealthComponent* healthComp = entity->createComp<HealthComponent>();
	if (!healthComp && entity->hasComp<HealthComponent>())
		healthComp = entity->getComp<HealthComponent>();
	if (!healthComp) return;

	if (!table["maxHealth"].isNil() && table["maxHealth"].isNumber())
		healthComp->setMaxHealth(table["maxHealth"].cast<float>());
	if (!table["currentHealth"].isNil() && table["currentHealth"].isNumber())
		healthComp->setCurrentHealth(table["currentHealth"].cast<float>());

}



void ComponentArranger::readAnimationComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	std::string animScriptFileName = table["animScriptDir"].cast<std::string>();
	if (luaL_dofile(mLuaState, animScriptFileName.c_str()) != 0){
		std::cout << lua_tostring(mLuaState, -1) << std::endl;
		return;
	}

	lua_pcall(mLuaState, 0, 0, 0);

	luabridge::LuaRef animTable = luabridge::getGlobal(mLuaState, "Animations");


	AnimationComponent* animationComp = entity->createComp<AnimationComponent>();
	animationComp->setTextureStringManager(&mTexturesStringManager);



	int i = 1;
	while (!animTable[i].isNil() && animTable[i].isTable()){
		luabridge::LuaRef curIndex = animTable[i].cast<luabridge::LuaRef>();
		AnimationData animData;
		
		std::string animName = curIndex["name"].cast<std::string>();
		animData.wholeDuration = sf::seconds(curIndex["wholeDuration"].cast<float>());
		animData.endToStart = curIndex["endToStart"].cast<bool>();
		animData.repeat = curIndex["isRepeating"].cast<bool>();
		animData.textureLocation = curIndex["textureLocation"].cast<std::string>();
		animData.flipX = curIndex["flipX"].cast<bool>();
		animData.transformOrigin = sf::Vector2f(-1.f, -1.f);

		if (!curIndex["is_dead_animation"].isNil())
			animData.m_is_dead_animation = curIndex["is_dead_animation"].cast<bool>();

		if (!curIndex["transformOrigin"].isNil() && curIndex["transformOrigin"].isTable()){
			luabridge::LuaRef originTable = curIndex["transformOrigin"];

			animData.transformOrigin.x = originTable[1].cast<float>();
			animData.transformOrigin.y = originTable[2].cast<float>();
		}
		

		luabridge::LuaRef wholeFrameSizeTable = curIndex["wholeFrameSize"].cast<luabridge::LuaRef>();
		animData.wholeFrameSize = sf::Vector2i(wholeFrameSizeTable[1].cast<int>(), 
			wholeFrameSizeTable[2].cast<int>());


		int origIndex = 1;
		luabridge::LuaRef origListTable = curIndex["origLists"].cast<luabridge::LuaRef>();

		//Animation::OrigList origList;
		while (!origListTable[origIndex].isNil()){
			luabridge::LuaRef curOrigTable = origListTable[origIndex].cast<luabridge::LuaRef>();
			luabridge::LuaRef intRectT = curOrigTable["frame"].cast<luabridge::LuaRef>();

			animData.origList.push_back(std::make_pair(
				sf::IntRect(intRectT[1], intRectT[2], intRectT[3], intRectT[4]),
				sf::seconds(curOrigTable["duration"].cast<float>())
				));

			origIndex++;
		}

		animationComp->insertNewAnimData(animName, animData);
		i++;

	}
		
}



void ComponentArranger::readTextDisplayComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	entity->createComp<TextDisplayComponent>();
}



void ComponentArranger::readScriptUpdateComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	ScriptUpdateComponent* scriptComponent = entity->createComp<ScriptUpdateComponent>();
	if (!scriptComponent && entity->hasComp<ScriptUpdateComponent>())
		scriptComponent = entity->getComp<ScriptUpdateComponent>();
	if (!scriptComponent) return;

	
	scriptComponent->setScriptUpdateFunc(table["updateFunc"], mLuaState);
	scriptComponent->setScriptMemberVariables(
		mLuaUtilityTableFunc["deepTableCopy"](table["memberVariables"])
		);
}



void ComponentArranger::readTimerComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	TimerComponent* timerComp = entity->createComp<TimerComponent>();
	if (!timerComp && entity->hasComp<TimerComponent>())
		timerComp = entity->getComp<TimerComponent>();
	if (!timerComp) return;

	luabridge::LuaRef timerList = table["timerList"].cast<luabridge::LuaRef>();

	int index = 1;

	while (!timerList[index].isNil()){
		luabridge::LuaRef curTimer = timerList[index].cast<luabridge::LuaRef>();
		TimerData timerData;
		timerData.mElapsedTime = sf::seconds(curTimer["elapsedTime"].cast <float>());
		timerData.mTimeLimit = sf::seconds(curTimer["timeLimit"].cast <float>());
		timerComp->mTimerDataList.insert(std::make_pair(curTimer["name"].cast<std::string>(), timerData));

		index++;
	}
}




void ComponentArranger::readDestroyableComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	DestroyableComponent* destroyableComp = entity->createComp<DestroyableComponent>();
	if (!destroyableComp && entity->hasComp<DestroyableComponent>())
		destroyableComp = entity->getComp<DestroyableComponent>();
	if (!destroyableComp) return;

	destroyableComp->mLuaDestroyedFunc = std::make_unique<luabridge::LuaRef>(table["isDestroyed"]);
	destroyableComp->mLuaRemoveableFunc = std::make_unique<luabridge::LuaRef>(table["isRemoveable"]);
}




void ComponentArranger::readPlayerComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	PlayerComponent* playerComp = entity->createComp<PlayerComponent>();
	playerComp->m_cur_drain_power_rate = table["cur_drain_power_rate"].cast<float>();
	playerComp->m_min_drain_power_rate = table["min_drain_power_rate"].cast<float>();
	playerComp->m_max_drain_power_rate = table["max_drain_power_rate"].cast<float>();

	playerComp->m_max_power = table["max_power"].cast<float>();
	playerComp->m_cur_power = table["cur_power"].cast<float>();
}





void ComponentArranger::readBody2dComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	Body2dComponent* bodyComp = entity->createComp<Body2dComponent>();
	if (!bodyComp && entity->hasComp<Body2dComponent>())
		bodyComp = entity->getComp<Body2dComponent>();
	if (!bodyComp)return;
	
	if (!table["rotateBasedOnVelo"].isNil())
		bodyComp->mRotateBasedOnVelo = table["rotateBasedOnVelo"].cast<bool>();


	if (!table["steering_turning_rate"].isNil())
		bodyComp->m_steering_turning_rate = table["steering_turning_rate"].cast<float>();


	bodyComp->mBaseSpeed = table["baseSpeed"].cast<float>();
	b2World* bWorld = mGeneralData->mB2World.get();

	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_staticBody;
	if (!table["bodyType"].isNil()){
		std::string bodyTypeStr = Utility::transformStrToLowerCase(table["bodyType"].cast<std::string>());
		if (bodyTypeStr == "dynamic") bodyDef.type = b2BodyType::b2_dynamicBody;
		if (bodyTypeStr == "kinematic") bodyDef.type = b2BodyType::b2_kinematicBody;
	}
	
	if (!table["positionX"].isNil())
		bodyDef.position.x = Utility::formatPixelToMeter(table["positionX"].cast<float32>());
	if (!table["positionY"].isNil())
		bodyDef.position.y = Utility::formatPixelToMeter(table["positionY"].cast<float32>());

	if (!table["isBullet"].isNil())
		bodyDef.bullet = table["isBullet"].cast<bool>();
	if (!table["isBodyActive"].isNil())
		bodyDef.active = table["isBodyActive"].cast<bool>();

	float gravityScale = 1.f;
	if (!table["gravityScale"].isNil())
		gravityScale = table["gravityScale"].cast<float>();

	bodyDef.gravityScale = gravityScale;
	bodyDef.userData = entity;
	bodyComp->mBody = bWorld->CreateBody(&bodyDef);

	//b2FixtureDef fixtureDef;
	std::unique_ptr<b2Shape> baseShapePtr;
	float mass = table["mass"].isNil() ? 1.0f : table["mass"].cast<float>();
	b2MassData massData;
	massData.mass = mass;
	massData.center = b2Vec2(0.f, 0.f);
	massData.I = 0.f;
	
	//bodyComp->mBody->SetMassData(&massData);

	const std::size_t maxFixtureReserve = 20;

	if (!table["fixtures"].isNil()){
		int fixtureIndex = 1;
		luabridge::LuaRef fixtureListTable = table["fixtures"].cast<luabridge::LuaRef>();
		bodyComp->mFixtureLogicList.reserve(maxFixtureReserve);
		while (!fixtureListTable[fixtureIndex].isNil()){
			b2FixtureDef fixtureDef;
			
			bodyComp->mFixtureLogicList.push_back(FixtureLogic());
			//bodyComp->mFixtureLogicList[fixtureIndex - 1] = FixtureLogic();
			FixtureLogic& curFixtureLogic = bodyComp->mFixtureLogicList.back();

			luabridge::LuaRef fixtureTable = fixtureListTable[fixtureIndex].cast<luabridge::LuaRef>();
			fixtureDef.density = fixtureTable["density"].cast<float32>();
			fixtureDef.friction = fixtureTable["friction"].cast<float32>();
			fixtureDef.restitution = fixtureTable["restitution"].cast<float32>();
			
			curFixtureLogic.mFixtureName = "";
			if (!fixtureTable["name"].isNil())
				curFixtureLogic.mFixtureName =	fixtureTable["name"].cast<std::string>();

			if (!fixtureTable["drawColor"].isNil()){
				luabridge::LuaRef fixtureColorT = fixtureTable["drawColor"].cast<luabridge::LuaRef>();
				curFixtureLogic.mDrawColor = sf::Color(
					fixtureColorT[1].cast<std::size_t>(),
					fixtureColorT[2].cast<std::size_t>(),
					fixtureColorT[3].cast<std::size_t>(),
					fixtureColorT[4].cast<std::size_t>())
					;
			}

			if (!fixtureTable["isSensor"].isNil())
				fixtureDef.isSensor = fixtureTable["isSensor"].cast<bool>();

			bool isCurIndexMainFixture = false;
			if (!fixtureTable["isMainFixture"].isNil() && fixtureTable["isMainFixture"].cast<bool>()){
				bodyComp->mIndexOfMainFixture = fixtureIndex - 1;
				curFixtureLogic.mFixtureName = Body2dComponent::mMainFixtureName;
				isCurIndexMainFixture = true;
			}

			if (!fixtureTable["shape"].isNil()){
				luabridge::LuaRef shapeTable = fixtureTable["shape"].cast<luabridge::LuaRef>();
				readShapeTable(shapeTable, baseShapePtr, bodyComp, isCurIndexMainFixture);
			}
			fixtureDef.shape = baseShapePtr.get();


			fixtureDef.filter.categoryBits = fixtureTable["category"].cast<std::size_t>();
			std::size_t finalMaskBitsForThisFixture = 0;

			int maskBitIndex = 1;
			luabridge::LuaRef categoryToCollideTable = fixtureTable["categoryToBeCollided"].cast<luabridge::LuaRef>();
			
			while (!categoryToCollideTable[maskBitIndex].isNil()){
				finalMaskBitsForThisFixture |= categoryToCollideTable[maskBitIndex].cast<std::size_t>();
				maskBitIndex++;
			}
			fixtureDef.filter.maskBits = finalMaskBitsForThisFixture;
			curFixtureLogic.mOriCategoryToBeCollided = finalMaskBitsForThisFixture;

			if (!fixtureTable["categoryToBeCollisionSolved"].isNil()){
				int solveBitIndex = 1;

				luabridge::LuaRef categoryToBeCollisionSolvedTable =
					fixtureTable["categoryToBeCollisionSolved"].cast<luabridge::LuaRef>();
								
				while (!categoryToBeCollisionSolvedTable[solveBitIndex].isNil()){
					curFixtureLogic.mCategoryToBeCollisionSolved |=
						categoryToBeCollisionSolvedTable[solveBitIndex].cast<std::size_t>();
					solveBitIndex++;
				}
			}

			curFixtureLogic.mOriCategoryToBeCollisionSolved = 
				curFixtureLogic.mCategoryToBeCollisionSolved;

			if (!fixtureTable["interactionFunction"].isNil())
				curFixtureLogic.mInteractionLuaFunc = std::make_unique<luabridge::LuaRef>(
					fixtureTable["interactionFunction"].cast<luabridge::LuaRef>());

			if (!fixtureTable["beginContactFunction"].isNil())
				curFixtureLogic.mBeginContactLuaFunc = std::make_unique<luabridge::LuaRef>(
					fixtureTable["beginContactFunction"].cast<luabridge::LuaRef>());

			if (!fixtureTable["endContactFunction"].isNil())
				curFixtureLogic.mEndContactLuaFunc = std::make_unique<luabridge::LuaRef>(
					fixtureTable["endContactFunction"].cast<luabridge::LuaRef>());

			if (!fixtureTable["preSolveFunction"].isNil()){
				curFixtureLogic.mPreSolveLuaFunc = std::make_unique<luabridge::LuaRef>(
					fixtureTable["preSolveFunction"].cast<luabridge::LuaRef>());
			}
			if (!fixtureTable["postSolveFunction"].isNil()){
				curFixtureLogic.mPostSolveLuaFunc = std::make_unique<luabridge::LuaRef>(
					fixtureTable["postSolveFunction"].cast<luabridge::LuaRef>());
			}
			fixtureDef.userData = &curFixtureLogic;
			curFixtureLogic.mFixture = bodyComp->mBody->CreateFixture(&fixtureDef);
			
			
			if (!fixtureTable["isInteractionFixture"].isNil())
				curFixtureLogic.mIsInteractionFixture = fixtureTable["isInteractionFixture"].cast<bool>();
			if (!fixtureTable["isActive"].isNil()){
				curFixtureLogic.mIsActive = fixtureTable["isActive"].cast<bool>();
				if (!curFixtureLogic.mIsActive){
					b2Filter filterData = curFixtureLogic.mFixture->GetFilterData();
					filterData.maskBits = 0;
					curFixtureLogic.mFixture->SetFilterData(filterData);
					curFixtureLogic.mCategoryToBeCollisionSolved = 0;
				}
			}

			fixtureIndex++;
		}
		//std::cout << bodyComp->mBody->GetMass() << std::endl;
		//Utility::printVector(Utility::formatVecMeterToPixel(bodyComp->mBody->GetLocalCenter()), true); 
		bodyComp->mBody->SetMassData(&massData);

	}
	
	/*fixtureDef.userData = &bodyComp->mFixtureLogic;
	bodyComp->mFixtureLogic.mFixture = bodyComp->mBody->CreateFixture(&fixtureDef);*/
	
	
	//bWorld->CreateBody()
}

void ComponentArranger::readObstacleComponent(Entity* entity, luabridge::LuaRef& table)
{
	ObstacleComponent* obstacleComp = entity->createComp<ObstacleComponent>();
	if (!obstacleComp  && entity->hasComp<ObstacleComponent>())
		obstacleComp = entity->getComp<ObstacleComponent>();
	if (!obstacleComp) return;

	if (!table["isDestroyer"].isNil())
		obstacleComp->mIsDestroyer = table["isDestroyer"].cast<bool>();
}


void ComponentArranger::readShapeTable(const luabridge::LuaRef& shapeTable,
	std::unique_ptr<b2Shape>& baseShapePtr, Body2dComponent* bodyComp,
	bool changeBodyOrigin)
{
	std::string shapeType = Utility::transformStrToLowerCase(shapeTable["shapeType"].cast<std::string>());

	if (shapeType == "polygon"){
		luabridge::LuaRef polygonPointsTable = shapeTable["polygonPoints"];
		int index = 1;
		//baseShapePtr.reset(new b2PolygonShape());
		b2PolygonShape* polygonShape = new b2PolygonShape();
		baseShapePtr.reset(polygonShape);

		std::vector<b2Vec2> verticesList;
		while (!polygonPointsTable[index].isNil()){
			luabridge::LuaRef individualVerticeTable = polygonPointsTable[index].cast<luabridge::LuaRef>();

			b2Vec2 vec2;
			vec2.x = Utility::formatPixelToMeter(individualVerticeTable[1].cast<float32>());
			vec2.y = Utility::formatPixelToMeter(individualVerticeTable[2].cast<float32>());

			verticesList.push_back(vec2);
			index++;
		}
		if (verticesList.empty()){
			verticesList.push_back(Utility::formatVecPixelToMeter(sf::Vector2f(-16, -16)));
			verticesList.push_back(Utility::formatVecPixelToMeter(sf::Vector2f(16, -16)));
			verticesList.push_back(Utility::formatVecPixelToMeter(sf::Vector2f(16, 16)));
		}
		if (!verticesList.empty())
			polygonShape->Set(&(*verticesList.begin()), verticesList.size());

		float originPixelX = shapeTable["originX"].cast<float32>();
		float originPixelY = shapeTable["originY"].cast<float32>();

		if (bodyComp && changeBodyOrigin)
			bodyComp->mBodyShapeOrigin = sf::Vector2f(originPixelX, originPixelY);
	}
	else if (shapeType == "circle"){
		b2CircleShape *circleShape = new b2CircleShape();
		baseShapePtr.reset(circleShape);

		circleShape->m_radius = Utility::formatPixelToMeter(shapeTable["radius"].cast<float32>());

		float originPixelX = shapeTable["originX"].cast<float32>();
		float originPixelY = shapeTable["originY"].cast<float32>();

		circleShape->m_p = b2Vec2(Utility::formatPixelToMeter(originPixelX),
			Utility::formatPixelToMeter(originPixelY));

		//if (bodyComp && changeBodyOrigin)
			//bodyComp->mBodyShapeOrigin = sf::Vector2f(originPixelX, originPixelY);

	}
	else if (shapeType == "box"){
		b2PolygonShape* boxShape = new b2PolygonShape();
		baseShapePtr.reset(boxShape);
		//divided by 2 since Box2D takes the argument to be multiplied by two
		float boxWidthPixel = shapeTable["width"].cast<float32>();
		float boxHeightPixel = shapeTable["height"].cast<float32>();

		float32 boxWidthM = Utility::formatPixelToMeter(boxWidthPixel) / 2.f;
		float32 boxHeightM = Utility::formatPixelToMeter(boxHeightPixel) / 2.f;

		float originPixelX = shapeTable["originX"].cast<float32>();
		float originPixelY = shapeTable["originY"].cast<float32>();
		float32 originX = Utility::formatPixelToMeter(originPixelX);
		float32 originY = Utility::formatPixelToMeter(originPixelY);

		if (bodyComp && changeBodyOrigin)
			bodyComp->mBodyShapeOrigin = sf::Vector2f(boxWidthPixel / -2.f + originPixelX,
				boxWidthPixel / -2.f + originPixelY);

		boxShape->SetAsBox(boxWidthM, boxHeightM, b2Vec2(originX,
			originY), 0.f);
		//for (int i = 0; i < 4; i++)
		//Utility::printVector(Utility::formatVecMeterToPixel(boxShape->GetVertex(i)), true);
	}
	else if (shapeType == "edge"){
		b2EdgeShape* edgeShape = new b2EdgeShape();
		baseShapePtr.reset(edgeShape);

		luabridge::LuaRef pos1T = shapeTable["pos1"].cast<luabridge::LuaRef>();
		luabridge::LuaRef pos2T = shapeTable["pos2"].cast<luabridge::LuaRef>();

		b2Vec2 pos1Vec2(Utility::formatPixelToMeter(pos1T[1].cast<float>()), 
			Utility::formatPixelToMeter(pos1T[2].cast<float>()));

		b2Vec2 pos2Vec2(Utility::formatPixelToMeter(pos2T[1].cast<float>()),
			Utility::formatPixelToMeter(pos2T[2].cast<float>()));

		edgeShape->Set(pos1Vec2, pos2Vec2);
	}
}


void ComponentArranger::readAutoBody2dMovementComponent(Entity* entity, luabridge::LuaRef& table)
{
	AutoBody2dMovementComponent* autoBody2dComp = entity->createComp<AutoBody2dMovementComponent>();
	if (!autoBody2dComp && entity->hasComp<AutoBody2dMovementComponent>())
		autoBody2dComp = entity->getComp<AutoBody2dMovementComponent>();
	if (!autoBody2dComp) return;

	luabridge::LuaRef movementListT = table["autoMovementSetList"].cast<luabridge::LuaRef>();
	std::size_t movListIndexT = 1;

	while (!movementListT[movListIndexT].isNil()){

		luabridge::LuaRef curMovSetT = movementListT[movListIndexT].cast<luabridge::LuaRef>();

		std::string movementName = curMovSetT["movementName"].cast<std::string>();
		autoBody2dComp->mAutoMovementList.insert(std::make_pair(movementName, AutoMovementSetData()));

		AutoMovementSetData& autoMovSetData = autoBody2dComp->mAutoMovementList.find(movementName)->second;

		autoMovSetData.mIsStartingPosAbsolute = 
			curMovSetT["isStartingPosAbsolute"].cast<bool>();

		autoMovSetData.mUseStartingPos = curMovSetT["useStartingPos"].cast<bool>();

		if (!curMovSetT["startOffsetPos"].isNil()){
			luabridge::LuaRef startOffsetT = 
				curMovSetT["startOffsetPos"].cast<luabridge::LuaRef>();
			autoMovSetData.mStartOffsetPos.x = startOffsetT[1].cast<float>(); 
			autoMovSetData.mStartOffsetPos.y = startOffsetT[2].cast<float>();
		}

		if (!curMovSetT["reverseDataForNegativeXBaseMovDir"].isNil())
			autoMovSetData.mReverseDataForNegativeXBaseMovDir = 
				curMovSetT["reverseDataForNegativeXBaseMovDir"].cast<bool>();

		if (!curMovSetT["useLatestDegreeForOffset"].isNil()){
			autoMovSetData.mUseLatestDegreeForOffset = 
				curMovSetT["useLatestDegreeForOffset"].cast<bool>();

			luabridge::LuaRef latestDegreeOffsetPosT = 
				curMovSetT["latestDegreeOffsetPos"].cast<luabridge::LuaRef>();

			autoMovSetData.mLatestDegreeOffsetPos =	sf::Vector2f(latestDegreeOffsetPosT[1].cast<float>(),
				latestDegreeOffsetPosT[2].cast<float>());
		}

		autoMovSetData.mUseStartingDegree = curMovSetT["useStartingDegree"].cast<bool>();
		autoMovSetData.mStartOffsetDegree = curMovSetT["startOffsetDegree"].cast<float>();

		autoMovSetData.mUseRelativeToBaseMovDir = curMovSetT["useRelativeToBaseMovDir"].cast<bool>();
		autoMovSetData.mDegreeRelToBaseMovDir = curMovSetT["degreeRelativeToBaseMovDir"].cast<float>();
		if (!curMovSetT["posRelativeToBaseMovDir"].isNil()){
			luabridge::LuaRef posRelToMovDirT = curMovSetT["posRelativeToBaseMovDir"].cast<luabridge::LuaRef>();
			autoMovSetData.mPosRelToBaseMovDir.x = posRelToMovDirT[1].cast<float>();
			autoMovSetData.mPosRelToBaseMovDir.y = posRelToMovDirT[2].cast<float>();
		}

		if (!curMovSetT["shape"].isNil())
			readShapeTable(curMovSetT["shape"].cast<luabridge::LuaRef>(), autoMovSetData.mBaseShapePtr); 
		
		if (!curMovSetT["onDelayMoveFunction"].isNil()){
			autoMovSetData.mOnDelayMoveFunc = std::make_unique<luabridge::LuaRef>(
				curMovSetT["onDelayMoveFunction"].cast<luabridge::LuaRef>());
		}

		if (!curMovSetT["onActiveMoveFunction"].isNil()){
			autoMovSetData.mOnActiveMoveFunc = std::make_unique<luabridge::LuaRef>(
				curMovSetT["onActiveMoveFunction"].cast<luabridge::LuaRef>());
		}

		if (!curMovSetT["onDoneMoveFunction"].isNil()){
			autoMovSetData.mOnDoneMoveFunc = std::make_unique<luabridge::LuaRef>(
				curMovSetT["onDoneMoveFunction"].cast<luabridge::LuaRef>());
		}

		luabridge::LuaRef singleMovDataListT = curMovSetT["singleMoveDataList"].cast<luabridge::LuaRef>();
		std::size_t singleIndex = 1;
		while (!singleMovDataListT[singleIndex].isNil()){
			AutoSingleMoveData autoSingleMoveData;

			luabridge::LuaRef curSingleT = singleMovDataListT[singleIndex].cast<luabridge::LuaRef>();
			luabridge::LuaRef posT = curSingleT["offsetPos"];
			autoSingleMoveData.mOffsetPos.x = posT[1].cast<float>();
			autoSingleMoveData.mOffsetPos.y = posT[2].cast<float>();
			autoSingleMoveData.mOffsetDegree = curSingleT["offsetDegree"].cast<float>();
			autoSingleMoveData.mDelayDur = sf::seconds(curSingleT["delayDur"].cast<float>());
			autoSingleMoveData.mActiveDur =  sf::seconds(curSingleT["activeDur"].cast<float>());

			autoMovSetData.mSingleMoveDataList.push_back(autoSingleMoveData);
			singleIndex++;
		}



		std::size_t parentIndex = 1;
		luabridge::LuaRef parentMovDataList = curMovSetT["parentMoveDataList"].cast<luabridge::LuaRef>();
		while (!parentMovDataList[parentIndex].isNil()){
			AutoParentMoveData autoParentMoveData;

			luabridge::LuaRef curParentT = parentMovDataList[parentIndex].cast<luabridge::LuaRef>();
			luabridge::LuaRef posT = curParentT["offsetPos"];
			autoParentMoveData.mOffsetPos.x = posT[1].cast<float>();
			autoParentMoveData.mOffsetPos.y = posT[2].cast<float>();
			autoParentMoveData.mOffsetDegree = curParentT["offsetDegree"].cast<float>();
			autoParentMoveData.mDelayDur = sf::seconds(curParentT["delayDur"].cast<float>());
			autoParentMoveData.mActiveDur = sf::seconds(curParentT["activeDur"].cast<float>());

			autoMovSetData.mParentMoveDataList.push_back(autoParentMoveData);
			parentIndex++;
		}
		
		movListIndexT++;
		
	}
}


void ComponentArranger::readJumpComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	JumpComponent* jumpComp = entity->createComp<JumpComponent>();
	if (!jumpComp && entity->hasComp<JumpComponent>())
		jumpComp = entity->getComp<JumpComponent>();

	if (!jumpComp)
		return;

	jumpComp->mMaxNumberOfJumpLimit = table["maxNumberOfJumpLimit"].cast<float>();
	jumpComp->mJumpImpulse = table["jumpImpulse"].cast<float>();
	jumpComp->mMaxTimeJumpVeloHold = sf::seconds(table["maxTimeJumpVeloHold"].cast<float>());
	jumpComp->mJumpVeloHoldVal = table["jumpVeloHoldVal"].cast<float>();
	/*jumpComp->mMaxIncJumpPos = table["maxIncJumpPosition"].cast<float>();
	jumpComp->mIncJumpPosPerSec = table["incJumpPositionPerSec"].cast<float>();*/
}


void ComponentArranger::readRealTimeInputComponent(Entity* entity, luabridge::LuaRef& table)
{
	entity->createComp<RealTimeInputComponent>();
}


void ComponentArranger::readInstanceComponent(Entity* entity, luabridge::LuaRef& table)
{
	InstanceComponent* instanceComp = entity->createComp<InstanceComponent>();
	if (!instanceComp && entity->hasComp<InstanceComponent>())
		instanceComp = entity->getComp<InstanceComponent>();

	if (!instanceComp)
		return;

	instanceComp->mInstanceLuaFunc = std::make_unique<luabridge::LuaRef>(
		table["instanceFunc"].cast<luabridge::LuaRef>());
}


void ComponentArranger::readScriptAIComponent(Entity* entity,
	luabridge::LuaRef& table)
{
	ScriptAIComponent* scriptAIComp = entity->createComp<ScriptAIComponent>();
	if (!scriptAIComp && entity->hasComp<ScriptAIComponent>())
		scriptAIComp = entity->getComp<ScriptAIComponent>();

	std::string aiScriptFileName = table["AIScriptDir"].cast<std::string>();
	std::string aiTableStates = table["tableName"].cast<std::string>() + "AIStates";
	
	Utility::readLuaScriptFile(aiScriptFileName, mLuaState);
		
	scriptAIComp->mLuaState = mLuaState;

	luabridge::LuaRef states = luabridge::LuaRef::getGlobal(mLuaState, aiTableStates.c_str());

	states["initialStateFunc"](entity);
}


void ComponentArranger::readParticleEmitterComponent(Entity* entity, luabridge::LuaRef& table)
{
	ParticleEmitterComponent* comp = entity->createComp<ParticleEmitterComponent>();

	luabridge::LuaRef emitters_table = table["emitters_list"].cast<luabridge::LuaRef>();

	for (int i = 1; i <= emitters_table.length(); i++)
	{
		luabridge::LuaRef emitter_t = emitters_table[i].cast<luabridge::LuaRef>();

		std::string particle_system_name = emitter_t["particle_system_name"].cast<std::string>();
		bool is_infinite = emitter_t["is_infinite_life_time"].cast<bool>();

		sf::Time min_life_time = sf::seconds(emitter_t["min_life_time"].cast<float>());
		sf::Time max_life_time = sf::seconds(emitter_t["max_life_time"].cast<float>());

		sf::Vector2f local_pos(emitter_t["local_pos_x"].cast<float>(),
			emitter_t["local_pos_y"].cast<float>());
		float local_radius = emitter_t["local_radius"].cast<float>();

		sf::Vector2f particle_main_dir(emitter_t["particle_main_dir_x"].cast<float>(),
			emitter_t["particle_main_dir_y"].cast<float>());

		float dir_angle_deviation = emitter_t["dir_angle_deviation"].cast<float>();

		float min_angle_rotation = emitter_t["min_angle_rotation"].cast<float>();
		float max_angle_rotation = emitter_t["max_angle_rotation"].cast<float>();

		sf::Time emitter_life_time = sf::seconds(emitter_t["emitter_life_time"].cast<float>());

		float emitter_emission_rate = emitter_t["emission_rate"].cast<float>();

		float particle_magnitude = emitter_t["particle_magnitude"].cast<float>();
				

		comp->m_emitters_list.push_back(ParticleEmitterComponent::EmitterData(particle_system_name,
			emitter_life_time, is_infinite));

		ParticleEmitterComponent::EmitterData& emitter_data = comp->m_emitters_list.back();

		emitter_data.m_particle_magnitude = particle_magnitude;

		emitter_data.m_local_pos = local_pos;
		emitter_data.m_local_radius = local_radius;

		emitter_data.m_particle_main_dir = particle_main_dir;
		emitter_data.m_dir_angle_deviation = dir_angle_deviation;

		emitter_data.m_emitter.setEmissionRate(emitter_emission_rate);

		emitter_data.m_emitter.setParticleLifetime(
			thor::Distributions::uniform(min_life_time, max_life_time));

		emitter_data.m_emitter.setParticleVelocity(
			thor::Distributions::deflect(
			particle_main_dir * particle_magnitude, dir_angle_deviation));

		emitter_data.m_emitter.setParticleRotation(
			thor::Distributions::uniform(min_angle_rotation, max_angle_rotation));

		
	}
}


void ComponentArranger::readSpaceShipComponent(Entity* entity, luabridge::LuaRef& table)
{
	auto* space_ship_comp = entity->createComp<SpaceShipComponent>();

	if (!table["has_missile_ability"].isNil())
		space_ship_comp->m_has_missile = table["has_missile_ability"].cast<bool>();

	luabridge::LuaRef projectiles_table = table["projectiles_list"].cast<luabridge::LuaRef>();

	for (int i = 1; i <= projectiles_table.length(); i++)
	{
		luabridge::LuaRef projectile_t = projectiles_table[i].cast<luabridge::LuaRef>();

		space_ship_comp->m_projectiles_list.push_back(SpaceShipComponent::Projectile_Data());

		SpaceShipComponent::Projectile_Data& proj_data = 
			space_ship_comp->m_projectiles_list.back();

		proj_data.m_projectile_name = projectile_t["name"].cast<std::string>();
		proj_data.m_target_category = projectile_t["target_category"].cast<std::size_t>();
	}
}


void ComponentArranger::readPlanetSpawnComponent(Entity* entity, luabridge::LuaRef& table)
{
	entity->createComp<PlanetSpawnComponent>();
}
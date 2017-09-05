#include "ScriptUpdateSystem.h"
#include "../Component/ScriptUpdateComponent.h"
#include "../Entity/Entity.h"

ScriptUpdateSystem::ScriptUpdateSystem()
{
	pushRequiredComponent(ComponentIdentifier::ScriptUpdateComponent);
	//BaseSystem::setAdhereToBoundRect(true);
}


ScriptUpdateSystem::~ScriptUpdateSystem()
{
}


void ScriptUpdateSystem::processEntity(EntityManager* entityManager, Entity* entity, sf::Time dt)
{
	ScriptUpdateComponent* scriptUpdateComp = entity->getComp<ScriptUpdateComponent>();
	try{
		if (scriptUpdateComp->mScriptUpdateFunction.get() 
			&& scriptUpdateComp->mScriptMemberVariables.get())
		{
			(*scriptUpdateComp->mScriptUpdateFunction)(entityManager, entity, dt.asSeconds(), 
				*scriptUpdateComp->mScriptMemberVariables);
						
		}
		else if (scriptUpdateComp->mScriptUpdateFunction.get() 
			&& !scriptUpdateComp->mScriptMemberVariables.get())
		{
			(*scriptUpdateComp->mScriptUpdateFunction)(entityManager, entity, dt.asSeconds());
		}
	}
	catch (luabridge::LuaException& e){
		std::cout << e.what() << std::endl;
	}
}
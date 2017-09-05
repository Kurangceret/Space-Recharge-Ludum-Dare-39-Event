#include "EventManager.h"
#include "AllEventsHeader.h"

EventManager* EventManager::mEventManager = nullptr;

LuaEventCallBackHolder::LuaEventCallBackHolder(Entity* ownerEntity,
	const luabridge::LuaRef& luaEventReceiver)
	:mOwnerEntity(ownerEntity),
	mLuaEventReceiverFunc(luaEventReceiver)
{

}

const luabridge::LuaRef& LuaEventCallBackHolder::getLuaReceiverFunc()
{
	return mLuaEventReceiverFunc;
}

Entity* LuaEventCallBackHolder::getOwnerEntity()
{
	return mOwnerEntity;
}
BaseEventCallBackHolder::BaseEventCallBackHolder(std::uintptr_t ownerAddress)
:mOwnerAddress(ownerAddress)
{
}
std::uintptr_t BaseEventCallBackHolder::getOwnerAddress() const
{
	return mOwnerAddress;
}

/*EventHolderConnectionData::EventHolderConnectionData()
:mOwnerAddress(0), mConnectionID(0){}*/


EventManager::EventManager()
{
	registerAllEventsToLua();
}

EventManager::~EventManager()
{

}


void EventManager::registerAllEventsToLua()
{
	registerEventToLua<PrintEvent>("PrintEvent");
	registerEventToLua<PlayerCollidesSolidEntityEvent>("PlayerCollidesSolidEntityEvent");
	registerEventToLua<EntityCollidesEntityEvent>("EntityCollidesEntityEvent");
	registerEventToLua<InvasionEvent>("InvasionEvent");
}

void EventManager::cleanEventsAndListeners()
{
	while (!mQueueEvents.empty()) 
		mQueueEvents.pop();
	mListeners.clear();
}


EventManager* EventManager::getInstance()
{
	if (!mEventManager)
		mEventManager = new EventManager();
	return mEventManager;
}

void EventManager::queueScriptEvent(const std::string& eventName, BaseEvent* baseEvent, bool directCall)
{
	const auto& registeredEvent = getRegisteredEventToLuaData(eventName);
	if (!directCall){
		mQueueEvents.push(std::make_pair(registeredEvent.mEventRefID,
			std::move(registeredEvent.mEventDeriverFunc(*baseEvent))));
		return;
	}
	
	auto& findListener = mListeners.find(registeredEvent.mEventRefID);

	if (findListener == mListeners.end())
		return;

	BaseEvent::Ptr event(std::move(registeredEvent.mEventDeriverFunc(*baseEvent)));

	for (auto& eventCallBack : findListener->second)
		eventCallBack->call(event.get());
}

void EventManager::deleteListener(BaseEvent::RefID eventRefID, std::uintptr_t ownerAddress)
{
	auto iter = mListeners.find(eventRefID);
	if (iter == mListeners.end())
		return;

	BaseCallBackList& callBackList = iter->second;

	auto callBackIter = iterFromCallBackList(callBackList, ownerAddress);
	if (callBackIter != callBackList.end())
		callBackList.erase(callBackIter);
}

void EventManager::addLuaListener(Entity* thisEntity, const std::string& eventName,
	lua_State* luaState)
{
	const auto& registeredEvent = getRegisteredEventToLuaData(eventName);

	//auto& signalPair = signal_for(registeredEvent.mEventRefID);

	auto& eventCallBackList = signal_for(registeredEvent.mEventRefID);
	/*EventHolderConnectionData eventHolderData = registeredEvent.mEventCallBackDeriverFunc(
		*signalPair.first, thisEntity, luabridge::LuaRef::fromStack(luaState, 4));
	eventHolderData.mOwnerAddress = Utility::convertPointerToAddress(thisEntity);

	signalPair.second.push_back(eventHolderData);*/

	registeredEvent.mEventCallBackDeriverFunc(
		eventCallBackList, thisEntity, luabridge::LuaRef::fromStack(luaState, 4));
}

void EventManager::deleteLuaListener(Entity* thisEntity, const std::string& eventName)
{
	const auto& registeredEvent = getRegisteredEventToLuaData(eventName);
	deleteListener(registeredEvent.mEventRefID, Utility::convertPointerToAddress(thisEntity));
}

void EventManager::processQueuedEvents()
{
	while (!mQueueEvents.empty()){
		auto& topPair = mQueueEvents.front();

		auto findListener = mListeners.find(topPair.first);
		if (findListener == mListeners.end()){
			mQueueEvents.pop();
			continue;
		}

		for (auto& eventCallBack : findListener->second)
			eventCallBack->call(topPair.second.get());
		//findListener->second.first->emit(topPair.second.get());
		mQueueEvents.pop();
	}
}

const RegisterEventToLuaData& EventManager::getRegisteredEventToLuaData(const std::string& eventName) const
{
	auto find = mRegisteredEventToLuaList.find(eventName);
	assert(find != mRegisteredEventToLuaList.end());
	return find->second;
}

BaseCallBackList& EventManager::signal_for(BaseEvent::RefID eventRefId)
{
	auto find = mListeners.find(eventRefId);
	/*if (find == mListeners.end())
		mListeners.insert(std::make_pair(eventRefId,
		std::make_pair(std::make_unique<EventSignal>(),
		std::vector<EventHolderConnectionData>())
		));*/
	if (find == mListeners.end())
		mListeners.insert(std::make_pair(eventRefId, BaseCallBackList()
		));

	return mListeners[eventRefId];
}


BaseCallBackList::const_iterator EventManager::iterFromCallBackList(
	BaseCallBackList& callBackList, std::uintptr_t ownerAddress) const
{
	auto finalIter = callBackList.end();
	int i = 0;

	for (auto& callBack : callBackList){
		if (callBack->getOwnerAddress() == ownerAddress)
			break;
		i++;
	}

	if (i != callBackList.size())
		finalIter = callBackList.begin() + i;

	return finalIter;
}
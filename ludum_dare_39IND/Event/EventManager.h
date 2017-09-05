#pragma once
#include <queue>
#include <functional>
#include <unordered_map>
#include <cstdint>

#include "../Helper/Utility.h"
#include "BaseEvent.h"
#include "Event.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include <RefCountedPtr.h>



#include "simplesignal.h"

typedef Simple::Signal<void(const void*)> EventSignal;
typedef std::unique_ptr<EventSignal> EventSignalPtr;

class Entity;


template <class T>
class EngineEventCallBackHolder{
public:
	typedef typename std::function<void(const T*)> EventReceiverFunc;
	typedef typename std::unique_ptr<EngineEventCallBackHolder<T>> Ptr;
public:
	EngineEventCallBackHolder<T>(const	EventReceiverFunc& eventReceiverFunc)
		:mEventReceiverFunc(eventReceiverFunc)
	{
		mEventReceiverFunc = eventReceiverFunc;
	}
	
	~EngineEventCallBackHolder() = default;

	void operator()(const T* event)
	{ 
		mEventReceiverFunc(event); 
	}
	//std::uintptr_t getOwnerAddress(){ return mOwnerAddress; }
private:
	
	EventReceiverFunc mEventReceiverFunc;
};


class LuaEventCallBackHolder{
public:
	typedef std::unique_ptr<LuaEventCallBackHolder>Ptr;
public:
	LuaEventCallBackHolder(Entity* ownerEntity, 
		const luabridge::LuaRef& luaEventReceiver);

	const luabridge::LuaRef& getLuaReceiverFunc();
	Entity* getOwnerEntity();
private:

	luabridge::LuaRef mLuaEventReceiverFunc;
	Entity* mOwnerEntity;
};



class BaseEventCallBackHolder{
public:
	BaseEventCallBackHolder(std::uintptr_t ownerAddress);
	virtual ~BaseEventCallBackHolder() = default;
	typedef std::unique_ptr<BaseEventCallBackHolder> Ptr;
	
	std::uintptr_t getOwnerAddress() const;
	virtual void call(const void *event) = 0;
private:
	std::uintptr_t mOwnerAddress;
};

typedef std::vector<BaseEventCallBackHolder::Ptr> BaseCallBackList;


template <typename E>
class EventCallBackHolder : public BaseEventCallBackHolder{
public:
	typedef typename std::unique_ptr<EventCallBackHolder<E>> Ptr;
public:
	EventCallBackHolder(std::uintptr_t ownerAddress, EngineEventCallBackHolder<E>* engineEventHolderPtr,
		LuaEventCallBackHolder* luaEventHolderPtr)
		:BaseEventCallBackHolder(ownerAddress),
		mLuaEventCallBackHolder(luaEventHolderPtr),
		mEngineEventCallBackHolder(engineEventHolderPtr){}

	EventCallBackHolder(EventCallBackHolder& right)
		:
		mLuaEventCallBackHolder(std::move(right.mLuaEventCallBackHolder)),
		mEngineEventCallBackHolder(std::move(right.mEngineEventCallBackHolder))
	{}

	virtual ~EventCallBackHolder()
	{ 
		/*if (mEngineEventCallBackHolder) delete mEngineEventCallBackHolder;
		if (mLuaEventCallBackHolder) delete mLuaEventCallBackHolder;*/
	}

	virtual void call(const void* event);
	void operator()(const void *event){};

	//void setSignalID(std::size_t signalId){ mEventSignalID = signalId; }
private:
	//std::size_t mEventSignalID;
	std::unique_ptr<LuaEventCallBackHolder> mLuaEventCallBackHolder;
		
	std::unique_ptr<EngineEventCallBackHolder<E>> mEngineEventCallBackHolder;
};


template<class E>
void EventCallBackHolder<E>::call(const void *event)
{
	const E* baseEvent = (static_cast<const E*>(event));

	if (mEngineEventCallBackHolder.get())
		(*mEngineEventCallBackHolder)(baseEvent);

	if (mLuaEventCallBackHolder.get()){
		try{
			mLuaEventCallBackHolder->getLuaReceiverFunc()
				(mLuaEventCallBackHolder->getOwnerEntity(), baseEvent);
		}
		catch (luabridge::LuaException& e){
			std::cout << e.what() << std::endl;
		}

	}
}

class EventManager;
struct EventHolderConnectionData;
typedef std::vector<BaseEventCallBackHolder> EventHolderLists;
//will be used in unordered_map with std::string as it's key
//
class RegisterEventToLuaData{
public:

	//template<class E>
	//void registerEventToLua();
	~RegisterEventToLuaData() = default;
private:
		
private:
	friend EventManager;
	RegisterEventToLuaData() = default;
	

	BaseEvent::RefID mEventRefID;
	std::function <BaseEvent::Ptr(BaseEvent&)> mEventDeriverFunc;
	//std::function <EventHolderConnectionData(EventSignal&, Entity*, const luabridge::LuaRef&)> mEventCallBackDeriverFunc;
	std::function <void(BaseCallBackList&, Entity*, const luabridge::LuaRef&)> mEventCallBackDeriverFunc;
};

/*struct EventHolderConnectionData{
	EventHolderConnectionData();
	typedef std::unique_ptr<EventHolderConnectionData> Ptr;

	std::uintptr_t mOwnerAddress;
	std::size_t mConnectionID;
	std::unique_ptr<BaseEventCallBackHolder> mBaseEventCallBackObject;
};*/

class EventManager{
public:
	~EventManager();

	static EventManager* getInstance();

	template <class T, typename ... Args>
	void queueEvent(Args&& ... args);

	template<class T>
	void queueEvent(std::unique_ptr<Event<T>> eventArgs);

	template<class T>
	void addListener(std::uintptr_t ownerAddress, EngineEventCallBackHolder<T>* ptr);

	template <class E>
	void deleteListener(std::uintptr_t ownerAddress);

	void deleteListener(BaseEvent::RefID eventRefID, std::uintptr_t ownerAddress);

	template <class T, typename ... Args>
	void addListener(std::uintptr_t ownerAddress, Args&& ... args);

	void queueScriptEvent(const std::string& eventName, BaseEvent* BaseEvent, bool directCall = false);

	void addLuaListener(Entity* thisEntity, const std::string& eventName,
		lua_State* luaState);

	void deleteLuaListener(Entity* thisEntity, const std::string& eventName);

	void processQueuedEvents();
	void cleanEventsAndListeners();
	
private:
	EventManager();
		
	BaseCallBackList::const_iterator iterFromCallBackList(
		BaseCallBackList& callBackList, std::uintptr_t ownerAddress) const;
	

	//BaseEvent::RefID getEventID(const std::string& eventName) const;
	const RegisterEventToLuaData& getRegisteredEventToLuaData(const std::string& eventName) const;

	/*template<class E>
	SignalPair& signal_for();

	SignalPair& signal_for(BaseEvent::RefID eventRefId);*/

	template<class E>
	BaseCallBackList& signal_for();

	BaseCallBackList& signal_for(BaseEvent::RefID eventRefId);

	template<class E>
	void registerEventToLua(const std::string& eventName);

	void registerAllEventsToLua();
private:
	std::queue<std::pair<BaseEvent::RefID, BaseEvent::Ptr>> mQueueEvents;
	std::unordered_map<BaseEvent::RefID, BaseCallBackList> mListeners;
	std::unordered_map<std::string, RegisterEventToLuaData> mRegisteredEventToLuaList;
	
	static EventManager* mEventManager;
};

template<class E>
void EventManager::registerEventToLua(const std::string& eventName)
{
	RegisterEventToLuaData registerEventToLuaData;
	registerEventToLuaData.mEventRefID = Event<E>::getRefID();

	registerEventToLuaData.mEventDeriverFunc = [&](BaseEvent& eventBase)
	{
		//return BaseEvent::Ptr(new Event<E>(dynamic_cast<Event<E>&>(eventBase)));
		return BaseEvent::Ptr(new E(dynamic_cast<E&>(eventBase)));
	};

	registerEventToLuaData.mEventCallBackDeriverFunc = [&](BaseCallBackList& eventHolderList,
		Entity* ownerEntity, const luabridge::LuaRef& luaEventReceiver) /*-> EventHolderConnectionData*/
	{
		/*EventHolderConnectionData eventHolder;
		eventHolder.mOwnerAddress = Utility::convertPointerToAddress(ownerEntity);*/

		std::unique_ptr<EventCallBackHolder<E>> eventCallBack(new EventCallBackHolder<E>(Utility::convertPointerToAddress(ownerEntity), 
			nullptr,
			new LuaEventCallBackHolder(ownerEntity, luaEventReceiver)));
		//eventCallBack.mOwnerAddress = Utility::convertPointerToAddress(ownerEntity);

		eventHolderList.push_back(std::move(eventCallBack));
		//eventHolder.mConnectionID = eventSignal.connect(eventCallBack);
		
	};

	mRegisteredEventToLuaList.insert(std::make_pair(eventName, registerEventToLuaData));
}

template<class E>
BaseCallBackList& EventManager::signal_for()
{
	BaseEvent::RefID eventRefId = Event<E>::getRefID();
	return signal_for(eventRefId);
}

template <class T, typename ... Args>
void EventManager::queueEvent(Args&& ... args)
{
	std::unique_ptr<Event<T>> event(new T(std::forward<Args>(args) ...));
	queueEvent<T>(std::move(event));
}

template<class T>
void EventManager::queueEvent(std::unique_ptr<Event<T>> eventArgs)
{
	mQueueEvents.push(std::make_pair(Event<T>::getRefID(), std::move(eventArgs)));
}

template<class T>
void EventManager::addListener(std::uintptr_t ownerAddress, EngineEventCallBackHolder<T>* ptr)
{	
	std::unique_ptr<EventCallBackHolder<T>> eventCallBack(new EventCallBackHolder<T>(ownerAddress, ptr, nullptr));
	//EventHolderConnectionData eventHolderData;

	BaseCallBackList& callBackList = signal_for<T>();
	
	auto findIter = iterFromCallBackList(callBackList, ownerAddress);
	if (findIter != callBackList.end())
		callBackList.erase(findIter);
		
	/*eventHolderData.mOwnerAddress = ptr->getOwnerAddress();

	eventHolderData.mBaseEventCallBackObject = std::move(eventCallBack).release();*/

	callBackList.push_back(std::move(eventCallBack));
}

template <class T, typename ... Args>
void EventManager::addListener(std::uintptr_t ownerAddress, Args&& ... args)
{
	addListener<T>(ownerAddress, new EngineEventCallBackHolder<T>(std::forward<Args>(args)...));
}

template <class E>
void EventManager::deleteListener(std::uintptr_t ownerAddress)
{
	deleteListener(Event<E>::getRefID(), ownerAddress);
	/*auto iter = mListeners.find(Event<E>::getRefID());
	if (iter == mListeners.end())
		return;

	BaseCallBackList& callBackList = iter->second;

	auto callBackIter = iterFromCallBackList(callBackList, ownerAddress);
	if (callBackIter != callBackList.end())
		callBackList.erase(callBackIter);*/
}


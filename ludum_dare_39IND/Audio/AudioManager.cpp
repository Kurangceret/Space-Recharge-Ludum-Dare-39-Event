#include "AudioManager.h"
#include "../Helper/Constant.h"
#include "../Helper/Utility.h"
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

AudioManager* AudioManager::mAudioManager = nullptr;

AudioManager::AudioManager()
{
}


AudioManager::~AudioManager()
{
}


AudioManager* AudioManager::getInstance()
{
	if (!mAudioManager)
		mAudioManager = new AudioManager();
	return mAudioManager;
}

void AudioManager::initializeAudioMap(lua_State* luaState)
{
	Utility::readLuaScriptFile(Constant::audioScriptDir + "AudioDirectoryData.lua", luaState);

	luabridge::LuaRef musicTable = luabridge::LuaRef::getGlobal(luaState, "IteratedMusicDirectoryData");

	for (int i = 1; i <= musicTable.length(); i++){
		luabridge::LuaRef curPair = musicTable[i].cast<luabridge::LuaRef>();
		mMusicMap.insert(std::make_pair(curPair[1].cast<std::string>(), 
			curPair[2].cast<std::string>()));
	}

	luabridge::LuaRef soundEffectTable = luabridge::LuaRef::getGlobal(luaState, "IteratedSoundEffectDirectoryData");

	for (int i = 1; i <= soundEffectTable.length(); i++){
		luabridge::LuaRef curPair = soundEffectTable[i].cast<luabridge::LuaRef>();

		SoundBufferPtr soundBuffer(new sf::SoundBuffer());

		std::string soundDir = curPair[2].cast<std::string>();

		if (!soundBuffer->loadFromFile(soundDir)){
			throw std::runtime_error("Sound effect dir: " + soundDir + " can't be loaded");
			continue;
		}

		mSoundEffectMap.insert(std::make_pair(curPair[1].cast<std::string>(), std::move(soundBuffer)));
	}
}

void AudioManager::playMusic(const std::string& musicKey, float volume)
{
	auto findIter = mMusicMap.find(musicKey);
	if (findIter == mMusicMap.end())
		return;
	if (!mCurMusic.openFromFile(findIter->second)){
		std::cout << "Music cannot be opened, \nMusic key : " + 
			musicKey + "\nMusic Dir:" + findIter->second << std::endl;
		return;
	}
	mCurMusic.setVolume(volume);
	mCurMusic.setLoop(true);
	mCurMusic.play();
}

void AudioManager::stopMusic()
{
	mCurMusic.stop();
}

void AudioManager::pauseMusic()
{
	mCurMusic.pause();
}

void AudioManager::resumeMusic()
{
	mCurMusic.play();
}

void AudioManager::playSoundEffect(const std::string& soundKey, float volume)
{
	auto findIter = mSoundEffectMap.find(soundKey);
	if (findIter == mSoundEffectMap.end())
		return;
	
	mSounds.push_back(sf::Sound(*findIter->second));

	auto& recentSound = mSounds.back();
	recentSound.setVolume(volume);
	recentSound.play();
}
#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <unordered_map>
#include <string>
#include <memory>

struct lua_State;


class AudioManager{
public:
	typedef std::unique_ptr<sf::SoundBuffer> SoundBufferPtr;
	typedef std::unordered_map<std::string, std::string> AudioMap;
	typedef std::unordered_map<std::string, SoundBufferPtr> SoundBufferMap;
public:
	AudioManager();
	~AudioManager();

	static AudioManager* getInstance();
	
	void initializeAudioMap(lua_State* luaState);
	void playMusic(const std::string& musicKey, float volume);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();

	void playSoundEffect(const std::string& soundKey, float volume);

private:
	static AudioManager* mAudioManager;

	
	SoundBufferMap mSoundEffectMap;
	//the key is sound effect alias name
	//second is the directory to those files
	AudioMap mMusicMap;

	sf::Music mCurMusic;
	std::vector<sf::Sound> mSounds;
};


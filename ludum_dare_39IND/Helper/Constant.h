#ifndef CONSTANT_H
#define CONSTANT_H
#include <string>
#include <SFML\System\Vector2.hpp>

namespace Constant{
	const std::string windowTitle = "Decoupling";

	const float windowWidth = 800.f;
	const float windowHeight = 600.f;

	const sf::Vector2f earthGravityDir = sf::Vector2f(0.f, 1.f);

	const std::string textureDir = "Media/Textures/";
	const std::string texture_ui_dir = textureDir + "UI/";
	const std::string texture_particle_dir = textureDir + "Particles/";
	const std::string fontDir = "Media/Fonts/";

	const std::string audioDir = "Media/Audio/";
	const std::string soundEffectDir = audioDir + "SoundEffects/";
	const std::string musicDir = audioDir + "Musics/";

	const std::string levelDir = "Levels/";
	const std::string scriptDir = "Scripts/";
	const std::string audioScriptDir = scriptDir + "AudioScripts/";
	const std::string stateEventScriptDir = scriptDir + "StateEventScripts/";
	const std::string settingScriptDir = scriptDir + "SettingScripts/";
	const std::string buffScriptDir = scriptDir + "BuffScripts/";
	const std::string debuffScriptDir = scriptDir + "DebuffScripts/";

	static std::string scriptScriptDir = scriptDir;
	const float pixelToMeterDiff = 30.f;
}
#endif
#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <memory>
#include <string>
#include <cstdlib>
#include <cassert>

//this class uses texture location as its id
class TextureStringManager{
public:
	TextureStringManager();
	~TextureStringManager();

	sf::Texture* loadTextures(const std::string& textureId, bool setRepeated = false);
	//const sf::Texture* loadTextures(const std::string& textureId) const;
	void deleteTexture(const std::string& key);

	//static TextureStringManager* getInstance();
private:
	//static TextureStringManager* mTextureStringManager;ss
	std::map<std::string, std::unique_ptr<sf::Texture> > mTexturesMap;
};


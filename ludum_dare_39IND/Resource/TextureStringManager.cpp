#include "TextureStringManager.h"


TextureStringManager::TextureStringManager()
{
}


TextureStringManager::~TextureStringManager()
{
}

/*TextureStringManager* TextureStringManager::mTextureStringManager = nullptr;

TextureStringManager* TextureStringManager::getInstance()
{
	if (!mTextureStringManager)
		mTextureStringManager = new TextureStringManager();
	return mTextureStringManager;
}*/

void TextureStringManager::deleteTexture(const std::string& key)
{
	auto iter = mTexturesMap.find(key);
	if (iter == mTexturesMap.end())
		return;

	delete iter->second.release();
	mTexturesMap.erase(iter);
}

sf::Texture* TextureStringManager::loadTextures(const std::string& textureId, bool setRepeated)
{
	auto iter = mTexturesMap.find(textureId);
	if (iter == mTexturesMap.end()){
		std::unique_ptr<sf::Texture> texture(new sf::Texture());

		if(!texture->loadFromFile(textureId)) 
			return nullptr;

		sf::Texture* tex = texture.get();
		mTexturesMap.insert(std::make_pair(textureId, std::move(texture)));
		tex->setRepeated(setRepeated);
		return tex;
	}
	iter->second->setRepeated(setRepeated);
	return iter->second.get();
}

/*const sf::Texture* TextureStringManager::loadTextures(const std::string& textureId) const
{
	return loadTextures(textureId);
}*/
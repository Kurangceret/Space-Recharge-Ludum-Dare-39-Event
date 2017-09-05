#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include <map>
#include <memory>
#include <string>
#include <cstdlib>
#include <cassert>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>



//this class will attempt to hold all type of resources into one (exception for sf::Music)
template<typename Resource, typename Identifier>
class ResourceHolder{
public:
	
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
	//this load will load the whole texture using the fileName
	void load(Identifier id, const std::string& fileName);
	//this function will load from image(usually from masked one)
	void loadFromImage(Identifier id, const sf::Image& image);
	//this function will take a reference from a newly made texture
	void loadFromResource(Identifier id, const Resource& resour);

	sf::Image* textureKeyToImg(const sf::Texture* key);
	//this load can either load the whole images or only a fragment of it(used mostly by sf::Shader)
	template<typename Parameter>
	void load(Identifier id, const std::string& fileName,
		const Parameter& secondParam);

	void destroyAllResources();
private:
	std::map<Identifier, std::unique_ptr<Resource> > mResourceMap;
	std::map<const sf::Texture*, sf::Image> mResourcesImageMap; // uses exclusively for sf::Texture resource type
};

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::destroyAllResources()
{
	for (auto& iter : mResourceMap){
		delete iter.second.release();
	}
	mResourceMap.clear();
	mResourcesImageMap.clear();
}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id,
	const std::string& fileName)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->loadFromFile(fileName))
		throw std::runtime_error("ResourceHolder:: load - Failed to Load " + fileName);

	//std::move takes the ownership from the variable texture to the argument of make_pair
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::loadFromResource(Identifier id,
	const Resource& resour)
{
	std::unique_ptr<Resource> resource(new Resource(resour));
	//if (!resource->loadFromFile(fileName))
		//throw std::runtime_error("ResourceHolder:: load - Failed to Load " + fileName);

	//std::move takes the ownership from the variable texture to the argument of make_pair
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template<typename Resource, typename Identifier>
sf::Image* ResourceHolder<Resource, Identifier>::textureKeyToImg(const sf::Texture* resource)
{
	auto iter = mResourcesImageMap.find(resource);
	if (iter == mResourcesImageMap.end())
		return nullptr;

	return &iter->second;
}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::loadFromImage(Identifier id,
	const sf::Image& image)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->loadFromImage(image))
		throw std::runtime_error("ResourceHolder:: load - Failed to Load  Image");
	//resource->update(image);
	mResourcesImageMap.insert(std::make_pair(resource.get(), image));
	//std::move takes the ownership from the variable texture to the argument of make_pair
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}


template<typename Resource, typename Identifier, typename Parameter>
void load(Identifier id, const std::string& fileName,
	const Parameter& secondParam)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->loadFromFile(fileName, secondParam))
		throw std::runtime_error("ResourceHolder:: load - Failed to Load " + fileName);

	//std::move takes the ownership from the variable texture to the argument of make_pair
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template<typename Resource, typename Identifier>
 Resource& ResourceHolder<Resource, Identifier>::get(
	Identifier id) 
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	//deference it to get the texture
	return *found->second;
}

template<typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(
	Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	//deference it to get the texture
	return *found->second;
}
#endif
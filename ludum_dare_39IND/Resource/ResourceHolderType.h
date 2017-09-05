#ifndef RESOURCEHOLDERTYPE_H
#define RESOURCEHOLDERTYPE_H
#include <string>
#include <SFML/Graphics/Texture.hpp>

template<typename Resource, typename Identifier>
class ResourceHolder;


typedef ResourceHolder<sf::Texture, unsigned int> TextureHolderInt;
typedef ResourceHolder<sf::Texture, std::string> TextureHolderStr;

#endif
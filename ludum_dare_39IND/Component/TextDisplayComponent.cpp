#include "TextDisplayComponent.h"
#include "ComponentIdentifier.h"

TextDisplayComponent::TextDisplayComponent(Entity* entity)
:Component(entity, ComponentIdentifier::TextDisplayComponent)
{
	
	mText.setCharacterSize(12);

}


TextDisplayComponent::~TextDisplayComponent()
{
}


sf::Text& TextDisplayComponent::getText()
{
	return mText;
}

void TextDisplayComponent::setString(const std::string& str)
{
	mText.setString(str);
}

void TextDisplayComponent::incrementString(const std::string& str)
{
	mText.setString(mText.getString() + "\n" + str);
}
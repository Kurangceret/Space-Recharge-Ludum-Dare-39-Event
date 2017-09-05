#include "VertexNodeComponent.h"
#include "ComponentIdentifier.h"

VertexNodeComponent::VertexNodeComponent(Entity* entity)
:Component(entity, ComponentIdentifier::VertexNodeComponent),
mHasInitialized(false),
mTileSetId(0),
mTileSet(nullptr),
mVertices(sf::Quads)
{
	
}


VertexNodeComponent::~VertexNodeComponent()
{
}

void VertexNodeComponent::drawVertexs(sf::RenderTarget& target) const
{
	sf::RenderStates states;
	states.texture = mTileSet;
	target.draw(mVertices, states);
}

void VertexNodeComponent::initializeTextureData(sf::Texture* texture, int tileSetId)
{
	if (mHasInitialized)
		return;

	mTileSet = texture;
	mTileSetId = tileSetId;
	mHasInitialized = true;
}

void VertexNodeComponent::addTile(const sf::Vector2f& pos, const sf::IntRect& rect)
{
	if (!mHasInitialized)
		return;

	//first corner, top left
	addVertex(pos.x - rect.width / 2, pos.y - rect.height / 2,
		rect.left, rect.top);
	//second corner, top right
	addVertex(pos.x + rect.width / 2, pos.y - rect.height / 2,
		rect.left + rect.width, rect.top);
	//third corner, bottom right
	addVertex(pos.x + rect.width / 2, pos.y + rect.height / 2,
		rect.left + rect.width, rect.top + rect.height);
	//fourth corner, bottom left
	addVertex(pos.x - rect.width / 2, pos.y + rect.height / 2,
		rect.left, rect.top + rect.height);

}

int VertexNodeComponent::getTileSetId() const
{
	return mTileSetId;
}

void VertexNodeComponent::addVertex(float posX, float posY, float texCoordX, float texCoordY)
{
	sf::Vertex v;
	v.position = sf::Vector2f(posX, posY);
	v.texCoords = sf::Vector2f(texCoordX, texCoordY);

	mVertices.append(v);
}

const sf::VertexArray& VertexNodeComponent::getVertices() const
{
	return mVertices;
}

const sf::Texture* VertexNodeComponent::getTileSet() const
{
	return mTileSet;
}
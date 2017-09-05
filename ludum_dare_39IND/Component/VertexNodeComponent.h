#pragma once
#include "Component.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>



class VertexNodeComponent :	public Component{
public:
	VertexNodeComponent(Entity* entity);
	virtual ~VertexNodeComponent();

	//func can only be called once
	void initializeTextureData(sf::Texture* texture, int tileSetId);

	void addTile(const sf::Vector2f& pos, const sf::IntRect& rect);
	int getTileSetId() const;

	void drawVertexs(sf::RenderTarget& target) const;

	const sf::VertexArray& getVertices() const;
	const sf::Texture* getTileSet() const;
private:
	void addVertex(float posX, float posY, float texCoordX, float texCoordY);
private:
	bool mHasInitialized;
	sf::Texture* mTileSet;
	int mTileSetId;
	sf::VertexArray mVertices;
};


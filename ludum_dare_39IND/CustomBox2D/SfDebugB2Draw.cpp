#include "SfDebugB2Draw.h"
#include "../Helper/Utility.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

SfDebugB2Draw::SfDebugB2Draw(sf::RenderTarget& renderTarget)
:mRenderTarget(renderTarget)
{
	AppendFlags(b2Draw::e_shapeBit);
}


SfDebugB2Draw::~SfDebugB2Draw()
{
}


void SfDebugB2Draw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	SfDebugB2Draw::DrawSolidPolygon(vertices, vertexCount, color);
}

/// Draw a solid closed polygon provided in CCW order.
void SfDebugB2Draw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::Color curColor(Utility::box2dColorToSfColor(color));
		
	//curColor = sf::Color(255, 255, 255);

	sf::VertexArray vertexs(sf::LinesStrip);
	
	sf::Vertex firstVertex;
	
	for (int i = 0; i < vertexCount; i++){
		const b2Vec2& curVertice = vertices[i];

		vertexs.append(sf::Vertex(sf::Vector2f(
			Utility::formatMeterToPixel(curVertice.x), 
			Utility::formatMeterToPixel(curVertice.y)), 
			curColor));
		if (i == 0)
			firstVertex = vertexs[i];
	}
	if (vertexCount > 2)
		vertexs.append(firstVertex);
	
	mRenderTarget.draw(vertexs);
}

/// Draw a circle.
void SfDebugB2Draw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	SfDebugB2Draw::DrawSolidCircle(center, radius, b2Vec2(), color);
}

/// Draw a solid circle.
void SfDebugB2Draw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	float pixelRadius = Utility::formatMeterToPixel(radius);
	sf::CircleShape circleShape;
	
	circleShape.setRadius(pixelRadius);
	circleShape.setPosition(sf::Vector2f(Utility::formatMeterToPixel(center.x),
		Utility::formatMeterToPixel(center.y)) + sf::Vector2f(-pixelRadius, -pixelRadius));
	circleShape.setOutlineThickness(1.f);
	circleShape.setOutlineColor(Utility::box2dColorToSfColor(color));
	circleShape.setFillColor(sf::Color::Transparent);

	mRenderTarget.draw(circleShape);
}


void SfDebugB2Draw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	sf::VertexArray vertexArray(sf::Lines);
	vertexArray.append(sf::Vertex(Utility::formatVecMeterToPixel(p1), 
		Utility::box2dColorToSfColor(color)));
	vertexArray.append(sf::Vertex(Utility::formatVecMeterToPixel(p2), 
		Utility::box2dColorToSfColor(color)));
	
	mRenderTarget.draw(vertexArray);
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void SfDebugB2Draw::DrawTransform(const b2Transform& xf)
{

}
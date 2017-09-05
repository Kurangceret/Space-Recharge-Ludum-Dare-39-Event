#pragma once

#include <Box2D\Common\b2Draw.h>
#include <SFML\Graphics\RenderTarget.hpp>

class SfDebugB2Draw :public b2Draw{
public:
	SfDebugB2Draw(sf::RenderTarget& renderTarget);
	virtual ~SfDebugB2Draw();

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf) override;
private:
	sf::RenderTarget& mRenderTarget;

};


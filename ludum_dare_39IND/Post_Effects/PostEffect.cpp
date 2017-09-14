#include "PostEffect.h"
#include <SFML/Graphics/VertexArray.hpp>

PostEffect::PostEffect()
{
}


PostEffect::~PostEffect()
{
}


void PostEffect::apply_shader(const sf::Shader& shader, sf::RenderTarget& output)
{
	sf::VertexArray quad_vertices(sf::TrianglesStrip, 4);
	sf::Vector2f output_size(output.getSize());

	quad_vertices.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 1.f)));
	quad_vertices.append(sf::Vertex(sf::Vector2f(output_size.x, 0.f), sf::Vector2f(1.f, 1.f)));
	quad_vertices.append(sf::Vertex(sf::Vector2f(0.f, output_size.y), sf::Vector2f(0.f, 0.f)));
	quad_vertices.append(sf::Vertex(output_size, sf::Vector2f(1.f, 0.f)));

	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;

	output.draw(quad_vertices, states);
}
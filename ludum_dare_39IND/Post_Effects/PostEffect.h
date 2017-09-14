#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

class PostEffect{
public:
	PostEffect();
	virtual ~PostEffect();

	virtual void apply(sf::RenderTexture& input, sf::RenderTarget& output) = 0;

protected:
	void apply_shader(const sf::Shader& shader, sf::RenderTarget& output);
};


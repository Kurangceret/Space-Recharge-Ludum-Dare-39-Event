#pragma once
#include "PostEffect.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <array>

class BloomEffect :	public PostEffect{
public:
	typedef std::array<sf::RenderTexture, 2> RenderTextureArray;
public:
	BloomEffect();
	virtual ~BloomEffect();

	void apply(sf::RenderTexture& input, sf::RenderTarget& output) override;

private:
	void init_textures(const sf::Vector2u& tex_size);

	void bright_pass(sf::RenderTexture& input, sf::RenderTexture& output);
	void downsample_tex(sf::RenderTexture& input, sf::RenderTexture& output);

	void blur_multipass(RenderTextureArray& tex_array);
	void blur_image(sf::RenderTexture& input, sf::RenderTexture& output, bool horizontal);

	void add_colors(sf::RenderTexture& first_tex, sf::RenderTexture& second_tex, 
		sf::RenderTarget& output);
private:

	sf::Shader m_blur_shader;
	sf::Shader m_downsample_tex_shader;
	sf::Shader m_add_shader;
	sf::Shader m_bright_pass_shader;

	sf::RenderTexture m_bright_texture;

	RenderTextureArray m_first_bright_texture_array;
	RenderTextureArray m_second_bright_texture_array;

};


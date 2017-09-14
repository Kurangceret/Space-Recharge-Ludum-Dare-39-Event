#include "BloomEffect.h"
#include "../Helper/Constant.h"

BloomEffect::BloomEffect()
{
	m_bright_pass_shader.loadFromFile(Constant::shader_dir + "Vert_VS.vert", 
		Constant::shader_dir + "Bright_Pass_FS.frag");

	m_add_shader.loadFromFile(Constant::shader_dir + "Vert_VS.vert",
		Constant::shader_dir + "Add_FS.frag");

	m_downsample_tex_shader.loadFromFile(Constant::shader_dir + "Vert_VS.vert",
		Constant::shader_dir + "Downsample_FS.frag");

	m_blur_shader.loadFromFile(Constant::shader_dir + "Vert_VS.vert",
		Constant::shader_dir + "Blur_FS.frag");
}


BloomEffect::~BloomEffect()
{
}


void BloomEffect::apply(sf::RenderTexture& input, sf::RenderTarget& output)
{
	init_textures(input.getSize());

	bright_pass(input, m_bright_texture);

	downsample_tex(m_bright_texture, m_first_bright_texture_array[0]);
	blur_multipass(m_first_bright_texture_array);

	downsample_tex(m_first_bright_texture_array[0], m_second_bright_texture_array[0]);
	blur_multipass(m_second_bright_texture_array);

	add_colors(m_first_bright_texture_array[0], 
		m_second_bright_texture_array[0], m_first_bright_texture_array[1]);

	m_first_bright_texture_array[1].display();
	add_colors(input, m_first_bright_texture_array[1], output);
	//add_colors(input, m_bright_texture, output);
}


void BloomEffect::init_textures(const sf::Vector2u& tex_size)
{
	if (m_bright_texture.getSize() == tex_size)
		return;
		
	m_bright_texture.create(tex_size.x, tex_size.y);
	m_bright_texture.setSmooth(true);

	m_first_bright_texture_array[0].create(tex_size.x / 2.f, tex_size.y / 2.f);
	m_first_bright_texture_array[0].setSmooth(true);
	m_first_bright_texture_array[1].create(tex_size.x / 2.f, tex_size.y / 2.f);
	m_first_bright_texture_array[1].setSmooth(true);


	m_second_bright_texture_array[0].create(tex_size.x / 4.f, tex_size.y / 4.f);
	m_second_bright_texture_array[0].setSmooth(true);
	m_second_bright_texture_array[1].create(tex_size.x / 4.f, tex_size.y / 4.f);
	m_second_bright_texture_array[1].setSmooth(true);


}


void BloomEffect::bright_pass(sf::RenderTexture& input, sf::RenderTexture& output)
{
	m_bright_pass_shader.setParameter("screen_texture", input.getTexture());
	apply_shader(m_bright_pass_shader, output);
	output.display();
}


void BloomEffect::downsample_tex(sf::RenderTexture& input, sf::RenderTexture& output)
{
	m_downsample_tex_shader.setParameter("source_tex", input.getTexture());
	m_downsample_tex_shader.setParameter("source_size", sf::Vector2f(input.getSize()));
	apply_shader(m_downsample_tex_shader, output);
	output.display();
}

void BloomEffect::blur_multipass(RenderTextureArray& tex_array)
{
	const static int sample_count = 2;

	bool horizontal = true;

	for (int i = 0; i < sample_count; i++)
	{
		blur_image(tex_array[0], tex_array[1], horizontal);
		blur_image(tex_array[1], tex_array[0], !horizontal);
	}
}

void BloomEffect::blur_image(sf::RenderTexture& input, sf::RenderTexture& output, bool horizontal)
{
	m_blur_shader.setParameter("image", input.getTexture());
	m_blur_shader.setParameter("horizontal", horizontal);
	apply_shader(m_blur_shader, output);
	output.display();
}

void BloomEffect::add_colors(sf::RenderTexture& first_tex, sf::RenderTexture& second_tex,
	sf::RenderTarget& output)
{
	m_add_shader.setParameter("source_texture", first_tex.getTexture());
	m_add_shader.setParameter("bloom_texture", second_tex.getTexture());
	apply_shader(m_add_shader, output);
	//output.display();
}
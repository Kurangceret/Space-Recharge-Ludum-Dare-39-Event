

uniform sampler2D source_texture;
uniform sampler2D bloom_texture;

void main()
{
	vec2 tex_coord = gl_TexCoord[0].xy;
	vec4 tex_color = texture2D(source_texture, tex_coord);
	vec4 bloom_color = texture2D(bloom_texture, tex_coord);

	gl_FragColor = tex_color + bloom_color;
	//gl_FragColor = tex_color;
	//gl_FragColor = bloom_color;
}

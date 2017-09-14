

uniform sampler2D source_tex;
uniform vec2 source_size;

vec2 offsets[8] = vec2[]
(
	vec2(-1.0, 1.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0),
	vec2(-1.0, 0.0),
	vec2(1.0, 0.0),
	vec2(-1.0, -1.0),
	vec2(0.0, -1.0),
	vec2(1.0, -1.0)
);

void main()
{
	vec2 pixel_size = vec2( 1.0 / source_size.x, 1.0 / source_size.y);
	vec2 tex_coord = gl_TexCoord[0].xy;
	vec4 color = texture2D(source_tex, tex_coord);

	for(int i = 0; i < 8; i++)
	{
		color += texture2D(source_tex, tex_coord + (offsets[i] * pixel_size));
	}

	gl_FragColor = (color / 9.0);
}



uniform sampler2D image;

uniform bool horizontal;


float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	//size of single texel
	vec2 tex_offset = 1.0 / textureSize(image , 0);
	vec2 tex_coord = gl_TexCoord[0].xy;

	vec3 result = texture2D(image, tex_coord).rgb * weight[0];

	if(horizontal)
	{
		for(int i = 1; i < 5; i++)
		{
			result += texture2D(image, tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture2D(image, tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
		
	}
	else
	{
		for(int i = 1; i < 5; i++)
		{
			result += texture2D(image, tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture2D(image, tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}


	}

	gl_FragColor = vec4(result, 1.0);
}
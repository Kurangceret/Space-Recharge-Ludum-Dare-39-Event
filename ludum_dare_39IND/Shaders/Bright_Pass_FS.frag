

uniform sampler2D screen_texture;

void main()
{
	vec4 tex_color = texture2D(screen_texture, gl_TexCoord[0].xy);

	float brightness = dot(tex_color.rgb, vec3(0.2126, 0.7152, 0.0722));

	vec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);

    if(brightness > 0.7)
        final_color = vec4(tex_color.rgb, 1.0);

	gl_FragColor = final_color;
	//gl_FragColor = vec4(brightness, brightness, brightness, 1.0);
	//gl_FragColor = tex_color;
}
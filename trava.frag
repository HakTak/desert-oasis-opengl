#version 330 core

in vec2 chTex; //koordinate teksture

out vec4 outCol;

uniform bool isInvisible;
uniform sampler2D uTex; //teksturna jedinica

void main()
{
	if(isInvisible)
		outCol = texture(uTex, chTex) * vec4(0.0, 0.0, 0.0, 0.0);
	else 
		outCol = texture(uTex, chTex);
}
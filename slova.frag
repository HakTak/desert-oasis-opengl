#version 330 core

in vec2 chTex; //koordinate teksture

out vec4 outCol;

uniform float uAlpha = 0.0;
uniform sampler2D uTex; //teksturna jedinica

void main()
{
	outCol = texture(uTex, chTex) * vec4(1.0, 1.0, 1.0, uAlpha);
}
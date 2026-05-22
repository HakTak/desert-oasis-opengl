#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje

out vec2 chTex;

uniform vec2 uPos;
uniform bool isReversed;

void main()
{
	gl_Position = vec4(inPos.x+uPos.x , inPos.y+uPos.y, 0.0, 1.0); //Od istoka ka zapadu
	if(isReversed)
		chTex = vec2(-inTex.x, inTex.y);
	else
		chTex = inTex;

}
#version 330 core 

layout(location = 0) in vec2 inPos; 

out vec4 chCol; 

uniform vec2 uPos;
uniform vec4 uCol;

void main() 
{
	gl_Position = vec4(inPos.x+uPos.x , inPos.y+uPos.y, 0.0, 1.0); //Od istoka ka zapadu
	chCol = uCol;
}
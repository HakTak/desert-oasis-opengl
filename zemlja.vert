#version 330 core 

layout(location = 0) in vec2 inPos; 

uniform vec4 uCol;

out vec4 chCol; 

void main() 
{
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); //Od istoka ka zapadu
	chCol = uCol;
}
#version 330 core 

layout(location = 0) in vec2 inPos; 
layout(location = 1) in vec3 inCol; 

out vec4 chCol; 

uniform float uY;

void main() 
{
	gl_Position = vec4(inPos.x, inPos.y + uY, 0.0, 1.0);		
	chCol = vec4(inCol, 1.0);

}
#version 330 core 

#define PI 3.141592
#define CRES 100

layout(location = 0) in vec2 inPos; 

uniform vec2 uPos;

out vec4 chCol; 

void main() 
{
	gl_Position = vec4(inPos.x * uPos.x - inPos.y * uPos.y, inPos.y * uPos.x + uPos.y * inPos.x, 0.0, 1.0); 
	chCol = vec4(0.9, 0.9, 0.8, -(uPos.y - 1)/2);
}
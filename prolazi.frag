#version 330 core 

in vec4 chCol; 
out vec4 outCol; 

uniform float uW;
uniform float uH;

void main() 
{
	if((2 * gl_FragCoord.y / uH - 1.0 >= -0.2 && 2 * gl_FragCoord.x / uW - 1.0 >= -0.29 && 2 * gl_FragCoord.x / uW - 1.0 <= -0.25)
		|| (2 * gl_FragCoord.y / uH - 1.0 >= -0.22 && 2 * gl_FragCoord.x / uW - 1.0 >= 0.475 && 2 * gl_FragCoord.x / uW - 1.0 <= 0.51)
		|| (2 * gl_FragCoord.y / uH - 1.0 >= -0.25 && 2 * gl_FragCoord.x / uW - 1.0 >= 0.1 && 2 * gl_FragCoord.x / uW - 1.0 <= 0.15)){ 
		outCol = chCol;
	}else{
		outCol = vec4(chCol.x, chCol.y, chCol.z, 0.0);
	}

}
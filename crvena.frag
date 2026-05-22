#version 330 core 

in vec4 chCol; 
out vec4 outCol; 

uniform float uW;
uniform float uH;

void main() 
{
	if((2 * gl_FragCoord.x / uW - 1.0 > 0.4 * (2 * gl_FragCoord.y / uH - 1.0) - 0.05)
		&& (2 * gl_FragCoord.x / uW - 1.0 < -0.7 * (2 * gl_FragCoord.y / uH - 1.0) + 0.225)
		&& (2 * gl_FragCoord.y / uH - 1.0 > -0.25) 
		&& mod(gl_FragCoord.x, 10) < 5){ 
		outCol = chCol;
	}else{
		outCol = vec4(chCol.x, chCol.y, chCol.z, 0.0);
	}
} 
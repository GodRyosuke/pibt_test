#version 330 core

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Color; 

out vec4 lineColor;
uniform mat4 cameraViewProj;

void main()
{
	gl_Position = cameraViewProj * vec4(Position, 1.0);
	lineColor = vec4(Color, 1.0);
}

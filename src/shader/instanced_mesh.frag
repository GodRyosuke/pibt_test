#version 450

out vec4 fragColor;
in vec4 diffuseColor;


void main()
{
    fragColor = diffuseColor;
}
#version 450

out vec4 fragColor;
in vec4 lineColor;

void main()
{
    fragColor = lineColor;
}
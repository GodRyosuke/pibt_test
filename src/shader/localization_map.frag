#version 450

uniform sampler2D diffuseTexture;  
in vec2 texCoord0;
out vec4 fragColor;

void main()
{
    fragColor = texture2D(diffuseTexture, texCoord0.xy); 
}
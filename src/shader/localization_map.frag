#version 450

uniform sampler2D diffuseTexture;  
in vec2 texCoord0;
out vec4 fragColor;

void main()
{
    vec4 texColor = texture2D(diffuseTexture, texCoord0.xy); 
    fragColor = vec4(texColor.r, texColor.r, texColor.r, 1.0);
}
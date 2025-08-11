#version 450 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 
layout (location = 3) in vec3 Tangent; 

struct InstancedMeshCompData
{
    mat4 modelTransform;
    vec4 color;
};

// Buffers
layout (std430, binding = 0) buffer InstancedMeshCompDataSBO
{
  InstancedMeshCompData instancedMeshCompDataArray[];
};

uniform mat4 cameraViewProj;

out vec2 texCoord0;
out vec3 normal0;
out vec3 tangent0;
out vec3 worldPos0;
out vec4 diffuseColor;

void main()
{
    InstancedMeshCompData instancedMeshCompData = instancedMeshCompDataArray[gl_InstanceID];
    mat4 modelTransform = instancedMeshCompData.modelTransform;
    
    worldPos0 = (modelTransform * vec4(Position, 1.0)).xyz;                          
	normal0 = (modelTransform * vec4(Normal, 0.0)).xyz;                            
	texCoord0 = TexCoord;
	tangent0 = (modelTransform * vec4(Tangent, 0.0)).xyz;
	gl_Position = cameraViewProj * modelTransform * vec4(Position, 1.0);
    diffuseColor = instancedMeshCompData.color;
}

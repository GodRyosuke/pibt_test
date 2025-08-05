#version 330 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 
layout (location = 3) in vec3 Tangent; 


uniform mat4 modelTransform;
// view projection matrixは頂点ごとで変化しないからcpuで一括uiniformのほうが早い
uniform mat4 cameraViewProj;
// SpotLight用
uniform mat4 lightView;
// uniform mat4 OrtholightViewProj;
uniform mat4 orthoLightProj;
// uniform mat4 LightProj;

// out vec4 OrthoLightSpacePos;                                                    
out vec2 texCoord0;
out vec3 normal0;
out vec3 tangent0;
out vec3 worldPos0;
out vec4 lightSpacePos0;

void main()
{
    worldPos0 = (modelTransform * vec4(Position, 1.0)).xyz;                          
	normal0 = (modelTransform * vec4(Normal, 0.0)).xyz;                            
	texCoord0 = TexCoord;
	tangent0 = (modelTransform * vec4(Tangent, 0.0)).xyz;
	gl_Position = cameraViewProj * modelTransform * vec4(Position, 1.0);

	// Directional Light
	lightSpacePos0 = orthoLightProj * lightView * modelTransform * vec4(Position, 1.0);
	// Spot Light
	// PersLightSpacePos = CameraProj * LightView * ModelTransform * vec4(Position, 1.0);
}

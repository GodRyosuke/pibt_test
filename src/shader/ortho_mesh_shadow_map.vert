#version 330 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 
layout (location = 3) in vec3 Tangent; 


uniform mat4 modelTransform;
// view projection matrixは頂点ごとで変化しないからcpuで一括uiniformのほうが早い
// SpotLight用
uniform mat4 lightView;
// uniform mat4 OrtholightViewProj;
uniform mat4 orthoLightProj;

void main()
{
	gl_Position = orthoLightProj * lightView * modelTransform * vec4(Position, 1.0);
}

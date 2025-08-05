#version 450

uniform vec4 color;
uniform vec3 eyeWorldPos;

in vec4 LightSpacePos;
in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

void main()
{
    vec3 lightDirection = vec3(1.0, 0.0, -1.0);
    vec3 ambientLight = vec3(0.2, 0.2, 0.2);
	vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
	vec3 specColor = vec3(0.5f, 0.5f, 1.f);
	float specPower = 200.f;

    // Surface normal
	vec3 N = normalize(normal0);
	vec3 L = normalize(-lightDirection);
	vec3 V = normalize(eyeWorldPos - worldPos0);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 phong = ambientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
        // 光が届かないところはそもそもspecの光沢が発生しない
        vec3 specular = specColor * pow(max(dot(R, V), 0.0), 1.2);
        phong += specular;
		vec3 diffuse = diffuseColor * NdotL;
		phong += diffuse;
	}

    float phongNorm = length(phong);
    float sqrt3 = sqrt(3);
    if (sqrt3 < phongNorm) {
        // もし(1.f, 1.f, 1.f)より大きなベクトルとなっていたらその大きさに抑える
        phong = phong * sqrt3 / phongNorm;
    }
    fragColor = vec4(phong, 1.0f);
}

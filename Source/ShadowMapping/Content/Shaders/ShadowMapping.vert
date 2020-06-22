#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

struct PLight
{
	vec4 Color;
	vec3 Position;
	float Radius;
};

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
uniform mat4 LightSpaceMatrix;
uniform PLight PointLight;

out VS_OUTPUT
{
	vec4 LightSpacePosition;
	vec3 WorldPosition;
	float Attenuation;
	vec3 Normal;
	vec2 TextureCoordinate;
} OUT;

void main()
{
	gl_Position = Projection * View * World * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.WorldPosition = (World * Position).xyz;
	OUT.Normal = transpose(inverse(mat3(World))) * Normal;
	OUT.LightSpacePosition = LightSpaceMatrix * vec4(OUT.WorldPosition, 1.0);
	
	vec3 direction = PointLight.Position - OUT.WorldPosition;
	OUT.Attenuation = clamp(1.0f - length(direction) / PointLight.Radius, 0.0f, 1.0f);
}
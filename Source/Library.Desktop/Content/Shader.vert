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
uniform PLight PointLight;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 WorldPosition;
	float Attenuation;
} OUT;

void main()
{
	gl_Position = Projection * View * World * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.WorldPosition = (World * Position).xyz;
	
	vec3 direction = PointLight.Position - OUT.WorldPosition;
	OUT.Attenuation = clamp(1.0f - length(direction) / PointLight.Radius, 0.0f, 1.0f);
}
#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

struct DLight
{
	vec4 Color;
	vec3 Direction;
};


uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
uniform DLight DirectionalLight;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
	DLight DirectionalLight;
} OUT;

void main()
{
	gl_Position = Projection * View * World * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.Normal = (World * vec4(Normal, 0.0f)).xyz;
	OUT.WorldPosition = (World * Position).xyz;

	OUT.DirectionalLight.Color = DirectionalLight.Color;
	OUT.DirectionalLight.Direction = -DirectionalLight.Direction;
}
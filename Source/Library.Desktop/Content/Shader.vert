#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
} OUT;

void main()
{
	gl_Position = Projection * View * World * Position;
	OUT.TextureCoordinate = TextureCoordinate;
}
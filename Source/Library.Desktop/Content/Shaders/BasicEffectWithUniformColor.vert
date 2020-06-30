#version 440 core

uniform mat4 WorldViewProjection;
uniform vec4 Color;

layout (location = 0) in vec4 Position;

out VS_OUTPUT
{
	vec4 Color;
} OUT;

void main()
{	
	gl_Position = WorldViewProjection * Position;
	OUT.Color = Color;
}
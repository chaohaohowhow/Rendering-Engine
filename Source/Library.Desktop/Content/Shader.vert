#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;

uniform mat4 projection;
uniform mat4 view;

out VS_OUTPUT
{
	vec4 Color;
} OUT;

void main()
{
	gl_Position = projection * view * Position;
	OUT.Color = Color;
}
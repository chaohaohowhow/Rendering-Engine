#version 440 core
layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

out VS_OUTPUT
{
    vec2 TextureCoordinate;
} OUT;

uniform mat4 WVP;

void main()
{
    OUT.TextureCoordinate = TextureCoordinate;
    gl_Position = WVP * Position;
}
#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

out VS_OUTPUT
{
    vec3 Position;
    vec2 TextureCoordinate;
    vec3 Normal;
} OUT;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    vec4 worldPosition = World * Position;
    gl_Position = Projection * View * worldPosition;
    OUT.Position = worldPosition.xyz;
    OUT.TextureCoordinate = TextureCoordinate;
    OUT.Normal = transpose(inverse(mat3(World))) * Normal;
}
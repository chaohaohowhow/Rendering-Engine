#version 440 core
layout (binding = 0) uniform sampler2D PositionTexture;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

struct PLight {
    vec3 Position;
    vec3 Color;
};

const int NumLights = 32;
uniform PLight PointLights[NumLights];
uniform float Linear = 0.7;
uniform float Quadratic = 1.8;

out VS_OUTPUT
{
    vec2 TextureCoordinate;
    float Attenuation[NumLights];
} OUT;

void main()
{
    OUT.TextureCoordinate = TextureCoordinate;
    gl_Position = Position;
    vec3 WorldPosition = texture(PositionTexture, TextureCoordinate).xyz;

    #pragma optionNV (unroll all)
    for(int i = 0; i < NumLights; ++i)
    {
        float distance = length(PointLights[i].Position - WorldPosition);
        OUT.Attenuation[i] = 1.0f / (1.0f + Linear * distance + Quadratic * distance * distance);
    }
}
#version 440 core
layout (binding = 0) uniform sampler2D PositionTexture;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

struct PLight {
    vec3 Position;
    vec3 Color;
};

const int NumLights = 32;
const float LightThreshold = 5.0/256.0;
uniform PLight PointLights[NumLights];
uniform float Constant = 1.0;
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
        float lightDistance = distance(PointLights[i].Position, WorldPosition);
        OUT.Attenuation[i] = 1.0f / (Constant + Linear * lightDistance + Quadratic * lightDistance * lightDistance);
    }
}
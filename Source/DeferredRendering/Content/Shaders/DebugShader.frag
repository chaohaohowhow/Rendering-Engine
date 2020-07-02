#version 440 core
out vec4 Color;
in vec2 TextureCoordinate;

layout (binding = 0) uniform sampler2D Sampler;

void main()
{
    Color = vec4(texture(Sampler, TextureCoordinate).rgb, 1.0f);
}
#version 440 core
layout (binding = 0) uniform sampler2D ScreenTexture;

out vec4 Color;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
} IN;


void main()
{
    vec3 col = texture(ScreenTexture, IN.TextureCoordinate).rgb;
    Color = vec4(col, 1.0f);
} 
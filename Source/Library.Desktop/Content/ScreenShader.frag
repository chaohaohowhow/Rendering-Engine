#version 440 core
layout (binding = 0) uniform sampler2D ScreenTexture;

uniform float Exposure;

out vec4 Color;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
} IN;

const float offset = 1.0 / 300.0;

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
);

float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

float edgeDetectionKernel[9] = float[] (
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
);

void main()
{
    vec3 sampleTex[9];
    for(uint i =0; i < 9; ++i)
    {
        sampleTex[i] = texture(ScreenTexture, IN.TextureCoordinate.st + offsets[i]).rgb;
    }
    vec3 col = vec3(0.0f);
    for(uint i = 0; i < 9; ++i)
    {
        col += sampleTex[i] * edgeDetectionKernel[i];
    }

    Color = vec4(col, 1.0f);
} 
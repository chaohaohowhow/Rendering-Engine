#version 440 core
layout (binding = 0) uniform sampler2D ScreenTexture;
layout (binding = 1) uniform sampler2D BloomBlur;

uniform bool Bloom = true;
uniform float Exposure = 1.0f;

layout (location = 0) out vec4 Color;

const float gamma = 2.2;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
} IN;

void main()
{
	vec3 hdrColor = texture(ScreenTexture, IN.TextureCoordinate).rgb;
	vec3 bloomColor = texture(BloomBlur, IN.TextureCoordinate).rgb;
	if(Bloom)
	{
		// additive blending
		hdrColor += bloomColor;
	}
	// Tone mapping
	vec3 result = vec3(1.0f) - exp(-hdrColor * Exposure);
	// Gamma correction
	result = pow(result, vec3(1.0 / gamma));
	Color = vec4(result, 1.0f);
} 
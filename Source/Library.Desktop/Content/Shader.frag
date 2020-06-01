#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;

uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 LightDirection;
	vec3 WorldPosition;
} IN;

out vec4 Color;

void main()
{
	vec3 normal = normalize(IN.Normal);
	vec3 viewDirection = normalize(CameraPosition - IN.WorldPosition);
	vec3 lightDirection = normalize(IN.LightDirection);
	float n_dot_l = dot(lightDirection, normal);

	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
	vec3 ambient = sampledColor.rgb * AmbientColor.rgb;
	vec3 directional = clamp(LightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);
	vec3 specular = SpecularColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), sampledColor.w);

	Color.rgb = ambient + directional + specular;
	Color.a = sampledColor.a;
}
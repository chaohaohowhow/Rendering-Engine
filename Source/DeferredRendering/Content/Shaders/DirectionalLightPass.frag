#version 440 core
out vec4 Color;
layout (binding = 0) uniform sampler2D PositionTexture;
layout (binding = 1) uniform sampler2D AlbedoSpecTexture;
layout (binding = 2) uniform sampler2D NormalTexture;

uniform vec4 DirectionalLightColor;
uniform vec3 DirectionalLightDirection;
uniform vec3 CameraPosition;
uniform float AmbientIntensity = 0.1f;
uniform float SpecularPower = 32.0f;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
} IN;

void main()
{
	vec3 Position = texture(PositionTexture, IN.TextureCoordinate).rgb;
	vec3 Normal = texture(NormalTexture, IN.TextureCoordinate).rgb;
	vec3 Diffuse = texture(AlbedoSpecTexture, IN.TextureCoordinate).rgb;
	float Specular = texture(AlbedoSpecTexture, IN.TextureCoordinate).a;

	// Calculate Lighting
	vec3 lighting = Diffuse * AmbientIntensity;

	vec3 viewDir = normalize(CameraPosition - Position);
	vec3 lightDir = normalize(DirectionalLightDirection);
	float n_dot_l = dot(Normal, lightDir);
	vec3 halfVector = normalize(lightDir + viewDir);
	float n_dot_h = dot(Normal, halfVector);
	
	vec3 diffuse = max(n_dot_l, 0.0f) * Diffuse * DirectionalLightColor.rgb;
	float spec = pow(max(n_dot_h, 0.0f), SpecularPower);
	vec3 specular = DirectionalLightColor.rgb * spec * Specular;

	lighting += diffuse + specular;
	Color = vec4(lighting, 1.0f);
}
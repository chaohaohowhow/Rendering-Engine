#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;
layout (binding = 1) uniform sampler2D ShadowMapSampler;

struct DLight
{
	vec4 Color;
	vec3 Direction;
};

struct PLight
{
	vec4 Color;
	vec3 Position;
	float Radius;
};

uniform vec4 AmbientColor;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;
uniform PLight PointLight;
uniform DLight DirectionalLight; // Direction is flipped on CPU

in VS_OUTPUT
{
	vec4 LightSpacePosition;
	vec3 WorldPosition;
	float Attenuation;
	vec3 Normal;
	vec2 TextureCoordinate;
} IN;

layout (location = 0) out vec4 Color;

vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
vec3 normal = normalize(IN.Normal);
vec3 viewDirection = normalize(CameraPosition - IN.WorldPosition);

vec3 CalculateByDirection(vec4 color, vec3 direction)
{
	vec3 lightDirection = normalize(direction);
	float n_dot_l = dot(lightDirection, normal);
	
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec3 diffuse = clamp(color.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);
	vec3 specular = SpecularColor.rgb * pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower); 
	return diffuse + specular;
}

vec3 CalculatePointLight(PLight light)
{
	vec3 direction = light.Position - IN.WorldPosition;
	
	return CalculateByDirection(light.Color, direction) * IN.Attenuation;
}

float CalculateShadow(vec4 lightSpacePosition, vec3 lightDir)
{
	// Persepective divide
	vec3 projectionCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	// Transform to [0, 1]
	projectionCoords = projectionCoords * 0.5 + 0.5;

	// Don't need closest depth with PCF
	//float closestDepth = texture(ShadowMapSampler, projectionCoords.xy).x;
	float currentDepth = projectionCoords.z;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = 0;
	vec2 texelSize = 1.0 / textureSize(ShadowMapSampler, 0);
	#pragma optionNV (unroll all)
	for(int x = -1; x <= 1; ++x)
	{
		#pragma optionNV (unroll all)
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(ShadowMapSampler, projectionCoords.xy + vec2(x, y) * texelSize).x;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0f;
	if(projectionCoords.z > 1.0f)
		shadow = 0.0;

	return shadow;
}

void main()
{
	vec3 ambient = sampledColor.rgb * AmbientColor.rgb;	
	vec3 diffuseSpecular = CalculateByDirection(DirectionalLight.Color, DirectionalLight.Direction);
	diffuseSpecular += CalculatePointLight(PointLight);
	float shadow = CalculateShadow(IN.LightSpacePosition, DirectionalLight.Direction);

	Color.rgb = ambient;
	Color.rgb += (1.0 - shadow) * (diffuseSpecular) * sampledColor.rgb;
	Color.a = sampledColor.a;
}
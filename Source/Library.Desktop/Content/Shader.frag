#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;
layout (binding = 1) uniform sampler2D NormalMapSampler;

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
	vec2 TextureCoordinate;
	vec3 WorldPosition;
	float Attenuation;
} IN;

layout (location = 0) out vec4 Color;
layout (location = 1) out vec4 BrightColor;

vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
vec3 normal = normalize(texture(NormalMapSampler, IN.TextureCoordinate).rgb * 2.0f - 1.0f);

vec3 CalculateByDirection(vec4 color, vec3 direction)
{
	vec3 viewDirection = normalize(CameraPosition - IN.WorldPosition);
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

void main()
{
	vec3 ambient = sampledColor.rgb * AmbientColor.rgb;	

	Color.rgb = ambient;
	Color.rgb += CalculateByDirection(DirectionalLight.Color, DirectionalLight.Direction);
	Color.rgb += CalculatePointLight(PointLight);
	Color.a = sampledColor.a;
	float brightness = dot(Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
	{
		BrightColor = Color;
	}
	else
	{
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}
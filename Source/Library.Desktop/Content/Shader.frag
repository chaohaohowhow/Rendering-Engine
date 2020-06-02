#version 440 core

layout (binding = 0) uniform sampler2D ColorTextureSampler;

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

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
	DLight DirectionalLight;
} IN;

out vec4 Color;

vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
vec3 normal = normalize(IN.Normal);

vec3 CalculateByDirection(vec4 color, vec3 direction)
{
	vec3 viewDirection = normalize(CameraPosition - IN.WorldPosition);
	vec3 lightDirection = normalize(direction);
	float n_dot_l = dot(lightDirection, normal);
	
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	vec3 diffuse = clamp(color.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);
	vec3 specular = SpecularColor.rgb * min(pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower), sampledColor.w);
	return diffuse + specular;
}

vec3 CalculatePointLight(PLight light)
{
	vec3 direction = light.Position - IN.WorldPosition;
	float attenuation = clamp(1.0f - length(direction) / light.Radius, 0.0f, 1.0f);
	
	return CalculateByDirection(light.Color, direction) * attenuation;
}

void main()
{
	vec3 ambient = sampledColor.rgb * AmbientColor.rgb;	

	Color.rgb = ambient;
	Color.rgb += CalculateByDirection(IN.DirectionalLight.Color, IN.DirectionalLight.Direction);
	Color.rgb += CalculatePointLight(PointLight);
	Color.a = sampledColor.a;
}
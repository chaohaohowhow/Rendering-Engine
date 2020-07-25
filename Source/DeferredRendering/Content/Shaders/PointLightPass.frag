#version 440 core
out vec4 Color;

layout (binding = 0) uniform sampler2D PositionTexture;
layout (binding = 1) uniform sampler2D AlbedoSpecTexture;
layout (binding = 2) uniform sampler2D NormalTexture;

uniform vec3 PointLightPosition;
uniform vec4 PointLightColor;
uniform float PointLightRadius;
uniform vec3 CameraPosition;
uniform float SpecularPower = 32.0f;
uniform float AmbientIntensity = 0.1f; // For showing sphere visualization
uniform vec2 ScreenSize = vec2(800, 600);

void main()
{
    vec2 texCoord = gl_FragCoord.xy / ScreenSize;
    // Retrieve data from gBuffer
    vec3 Position = texture(PositionTexture, texCoord).rgb;
    vec3 Normal = texture(NormalTexture, texCoord).rgb;
    vec3 Diffuse = texture(AlbedoSpecTexture, texCoord).rgb;
    float Specular = texture(AlbedoSpecTexture, texCoord).a;
    
    // Then calculate lighting as usual
    vec3 lighting = Diffuse * AmbientIntensity;
    vec3 viewDir  = normalize(CameraPosition - Position);
    
    vec3 lightDir = PointLightPosition - Position;
    float attenuation = max(1.0f - length(lightDir) / PointLightRadius, 0.0f);
    lightDir = normalize(lightDir);
    float n_dot_l = dot(Normal, lightDir);
    vec3 halfVector = normalize(lightDir + viewDir);
    float n_dot_h = dot(Normal, halfVector);

    vec3 diffuse = max(n_dot_l, 0.0f) * Diffuse * PointLightColor.rgb;

    float spec = pow(max(n_dot_h, 0.0f), SpecularPower);
    vec3 specular = PointLightColor.rgb * spec * Specular;
    lighting += (diffuse + specular) * attenuation;

    Color = vec4(lighting, 1.0);
}
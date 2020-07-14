#version 440 core
out vec4 Color;

layout (binding = 0) uniform sampler2D PositionTexture;
layout (binding = 1) uniform sampler2D AlbedoSpecTexture;
layout (binding = 2) uniform sampler2D NormalTexture;

struct PLight {
    vec3 Position;
    vec3 Color;
};
const int NumLights = 32;
uniform PLight PointLights[NumLights];
uniform vec3 CameraPosition;
uniform float SpecularPower = 16.0f;
uniform float AmbientIntensity = 0.5f;
uniform float Constant = 1.0;
uniform float Linear = 0.7;
uniform float Quadratic = 1.8;

in VS_OUTPUT
{
    vec2 TextureCoordinate;
} IN;

void main()
{
    // Retrieve data from gBuffer
    vec3 Position = texture(PositionTexture, IN.TextureCoordinate).rgb;
    vec3 Normal = texture(NormalTexture, IN.TextureCoordinate).rgb;
    vec3 Diffuse = texture(AlbedoSpecTexture, IN.TextureCoordinate).rgb;
    float Specular = texture(AlbedoSpecTexture, IN.TextureCoordinate).a;
    
    // Then calculate lighting as usual
    vec3 lighting = Diffuse * AmbientIntensity;
    vec3 viewDir  = normalize(CameraPosition - Position);
    
    #pragma optionNV (unroll all)
    for(int i = 0; i < NumLights; ++i)
    {
        vec3 lightDir = normalize(PointLights[i].Position - Position);
        float lightDistance = length(lightDir);
        float attenuation = 1.0f / (Constant + Linear * lightDistance + Quadratic * lightDistance * lightDistance);
        float n_dot_l = dot(Normal, lightDir);
        vec3 halfVector = normalize(lightDir + viewDir);
        float n_dot_h = dot(Normal, halfVector);

        vec3 diffuse = max(n_dot_l, 0.0f) * Diffuse * PointLights[i].Color;

        float spec = pow(max(n_dot_h, 0.0f), SpecularPower);
        vec3 specular = PointLights[i].Color * spec * Specular;

        lighting += (diffuse + specular) * attenuation;
    }
    Color = vec4(lighting, 1.0);
}
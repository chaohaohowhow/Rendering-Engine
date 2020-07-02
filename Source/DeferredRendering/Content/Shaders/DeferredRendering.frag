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

in VS_OUTPUT
{
    vec2 TextureCoordinate;
    float Attenuation[NumLights];
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
        float n_dot_l = dot(lightDir, Normal);
        vec3 halfVector = normalize(lightDir + viewDir);
        float n_dot_h = dot(Normal, halfVector);

        vec3 diffuse = clamp(PointLights[i].Color * n_dot_l * Diffuse, 0.0f, 1.0f);

        float spec = pow(clamp(n_dot_h, 0.0f, 1.0f), SpecularPower);
        vec3 specular = PointLights[i].Color * Specular * spec;

        lighting += (diffuse + specular) * IN.Attenuation[i];
    }
    Color = vec4(lighting, 1.0);
}
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
    vec3 lighting = Diffuse * 0.5;
    vec3 viewDir  = normalize(CameraPosition - Position);
    for(int i = 0; i < NumLights; ++i)
    {
        vec3 lightDir = normalize(PointLights[i].Position - Position);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * PointLights[i].Color;

        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = PointLights[i].Color * spec * Specular;

        diffuse *= IN.Attenuation[i];
        specular *= IN.Attenuation[i];
        lighting += diffuse + specular;
    }
    Color = vec4(lighting, 1.0);
}
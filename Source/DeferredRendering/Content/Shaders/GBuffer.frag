#version 440 core
layout (binding = 0) uniform sampler2D ColorTextureSampler;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec4 AlbedoSpec;
layout (location = 2) out vec3 Normal;

in VS_OUTPUT
{
    vec3 Position;
    vec2 TextureCoordinate;
    vec3 Normal;
} IN;

uniform int UseTexture;
uniform vec3 DiffuseColor;

void main()
{
    Position = IN.Position;
    Normal = normalize(IN.Normal);
    if(UseTexture != 0)
    {
        AlbedoSpec = texture(ColorTextureSampler, IN.TextureCoordinate);
    }
    else
    {
        AlbedoSpec = vec4(DiffuseColor, 1.0f);
    }
}
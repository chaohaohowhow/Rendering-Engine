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

void main()
{
    Position = IN.Position;
    Normal = normalize(IN.Normal);
    AlbedoSpec = texture(ColorTextureSampler, IN.TextureCoordinate);
}
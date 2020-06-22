#version 440 core
out vec4 Color;

in vec2 TextureCoordinate;

layout (binding = 0) uniform sampler2D ShadowMapSampler;
uniform float NearPlane;
uniform float FarPlane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NearPlane * FarPlane) / (FarPlane + NearPlane - z * (FarPlane - NearPlane));	
}

void main()
{             
    float depthValue = texture(ShadowMapSampler, TextureCoordinate).r;
    // Color = vec4(vec3(LinearizeDepth(depthValue) / FarPlane), 1.0); // perspective
    Color = vec4(vec3(depthValue), 1.0); // orthographic
}
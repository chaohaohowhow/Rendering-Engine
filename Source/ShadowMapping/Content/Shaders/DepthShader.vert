#version 440 core
layout (location = 0) in vec4 Position;

uniform mat4 LightSpaceMatrix;
uniform mat4 World;

void main()
{
    gl_Position = LightSpaceMatrix * World * Position;
}

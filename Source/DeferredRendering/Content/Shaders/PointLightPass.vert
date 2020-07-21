#version 440 core
layout (location = 0) in vec4 Position;

uniform mat4 WVP;

void main()
{
    gl_Position = WVP * Position;
}
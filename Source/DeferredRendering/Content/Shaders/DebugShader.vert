#version 440 core
layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoord;

out vec2 TextureCoordinate;

uniform mat4 Translate;

void main()
{
    TextureCoordinate = TextureCoord;
    gl_Position = Translate * Position;
}
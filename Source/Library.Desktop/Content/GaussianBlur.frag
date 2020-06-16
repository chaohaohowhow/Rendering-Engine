#version 440 core
out vec4 Color;
  
in VS_OUTPUT
{
	vec2 TextureCoordinate;
} IN;

uniform sampler2D image;
  
uniform bool Horizontal;
uniform float Weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, IN.TextureCoordinate).rgb * Weight[0]; // current fragment's contribution
    if(Horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, IN.TextureCoordinate + vec2(tex_offset.x * i, 0.0)).rgb * Weight[i];
            result += texture(image, IN.TextureCoordinate - vec2(tex_offset.x * i, 0.0)).rgb * Weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, IN.TextureCoordinate + vec2(0.0, tex_offset.y * i)).rgb * Weight[i];
            result += texture(image, IN.TextureCoordinate - vec2(0.0, tex_offset.y * i)).rgb * Weight[i];
        }
    }
    Color = vec4(result, 1.0);
}
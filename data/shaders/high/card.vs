#version 130

uniform mat4 MVPM;

in vec3 CardVertex;
in vec2 CardTextureCoordinate;
out vec2 TextureCoordinate;


void main()
{
    TextureCoordinate = CardTextureCoordinate;
    gl_Position = MVPM * vec4(CardVertex, 1);
}

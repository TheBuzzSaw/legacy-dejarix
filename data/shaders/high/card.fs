#version 130

uniform sampler2D CardTexture;
uniform bool UseTexture;
uniform vec4 CardColor;

in vec2 TextureCoordinate;
out vec4 gl_FragColor;

void main()
{
    if (UseTexture)
        gl_FragColor = vec4(texture(CardTexture, TextureCoordinate).rgb, 1.0);
    else
        gl_FragColor = CardColor;
}

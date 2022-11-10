#version 330 core
uniform sampler2D aTexture;

in vec4 Color;

out vec4 FragColor;

void main()
{
    vec2 Texture = vec2(Color.x + 0.5, Color.y + 0.5);
    FragColor = texture(aTexture, Texture);
}

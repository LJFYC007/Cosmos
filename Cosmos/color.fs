#version 330 core
uniform sampler2D aTexture;

in vec4 Color;

out vec4 FragColor;

void main()
{
    vec2 Texture = vec2(Color.x * 2, Color.y * 2);
    FragColor = texture(aTexture, Texture) * Color;
}

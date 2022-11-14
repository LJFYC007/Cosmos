#version 330 core
struct Material 
{ 
    sampler2D diffuse;
};
uniform Material material;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    FragColor = texture(material.diffuse, TexCoord);
}

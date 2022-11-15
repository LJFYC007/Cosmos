#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTangent;
layout (location = 2) in vec3 aBitangent;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec2 aTexCoord;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D height;

void main()
{
    vec3 Pos = aPos + 0.03 * texture(height, aTexCoord).x * aNormal;
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    vs_out.FragPos = vec3(model * vec4(Pos, 1.0));   
    vs_out.TexCoords = aTexCoord;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
}
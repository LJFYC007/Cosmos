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
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoord;
    vs_out.Normal = vec3(model * vec4(aNormal, 1.0));   
}
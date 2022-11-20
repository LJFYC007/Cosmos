#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
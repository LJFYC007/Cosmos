#pragma once
#include <glad/glad.h>
#include <stb_image.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

#include "shader.h"
#include "texture.h"

class Resource 
{
public: 
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader LoadShader(const char* vertexPath, const char* fragmentPath, std::string name);
	static Shader& GetShader(std::string name); 
	static Texture LoadTexture(const char* file, std::string name);
	static Texture GetTexture(std::string name);
	static void Clear()
	{
		for (auto it : Shaders) glDeleteProgram(it.second.ID);
		for (auto it : Textures) glDeleteTextures(1, &it.second.ID);
	}
		
private: 
	Resource(){}
	static Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath);
	static Texture loadTextureFromFile(const char* file);
};

std::map<std::string, Texture> Resource::Textures;
std::map<std::string, Shader> Resource::Shaders;

Shader Resource::LoadShader(const char* vertexPath, const char* fragmentPath, std::string name) {
	Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath);
	return Shaders[name];
}
Texture Resource::LoadTexture(const char* file, std::string name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}
Shader& Resource::GetShader(std::string name) { return Shaders[name]; }
Texture Resource::GetTexture(std::string name) { return Textures[name]; }

Shader Resource::loadShaderFromFile(const char* vertexPath, const char* fragmentPath)
{
	Shader shader(vertexPath, fragmentPath);
	return shader;
}

Texture Resource::loadTextureFromFile(const char* file)
{
	Texture texture;
	texture.Generate(file);
	return texture;
}


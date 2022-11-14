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
#include "mesh.h"

class Resource 
{
public: 
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
	static std::map<std::string, Mesh> Meshes;

	static Shader LoadShader(const char* vertexPath, const char* fragmentPath, std::string name);
	static Shader& GetShader(std::string name); 
	static Texture LoadTexture(const char* file, std::string name, std::string type);
	static Texture GetTexture(std::string name);
	static Mesh LoadMesh(std::vector<Vertex> vertecies, std::vector<unsigned int> indices, std::vector<Texture> textures, std::string name);
	static Mesh& GetMesh(std::string name);
	static void Clear()
	{
		for (auto it : Shaders) glDeleteProgram(it.second.ID);
		for (auto it : Textures) glDeleteTextures(1, &it.second.ID);
	}
		
private: 
	Resource(){}
	static Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath);
	static Texture loadTextureFromFile(const char* file, std::string type);
	static Mesh loadMesh(std::vector<Vertex> vertecies, std::vector<unsigned int> indices, std::vector<Texture> textures);
};

std::map<std::string, Texture> Resource::Textures;
std::map<std::string, Shader> Resource::Shaders;
std::map<std::string, Mesh> Resource::Meshes;

Shader Resource::LoadShader(const char* vertexPath, const char* fragmentPath, std::string name) {
	Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath);
	return Shaders[name];
}
Texture Resource::LoadTexture(const char* file, std::string name, std::string type)
{
	Textures[name] = loadTextureFromFile(file, type);
	return Textures[name];
}
Mesh Resource::LoadMesh(std::vector<Vertex> vertecies, std::vector<unsigned int> indices, std::vector<Texture> textures, std::string name)
{
	Meshes[name] = loadMesh(vertecies, indices, textures);
	return Meshes[name];
}
Shader& Resource::GetShader(std::string name) { return Shaders[name]; }
Texture Resource::GetTexture(std::string name) { return Textures[name]; }
Mesh& Resource::GetMesh(std::string name) { return Meshes[name]; }

Shader Resource::loadShaderFromFile(const char* vertexPath, const char* fragmentPath)
{
	Shader shader(vertexPath, fragmentPath);
	return shader;
}

Texture Resource::loadTextureFromFile(const char* file, std::string type)
{
	Texture texture;
	texture.Generate(file, type);
	return texture;
}

Mesh Resource::loadMesh(std::vector<Vertex> vertecies, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	Mesh mesh(vertecies, indices, textures);
	return mesh;
}


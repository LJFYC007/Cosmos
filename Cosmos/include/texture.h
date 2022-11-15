#pragma once
#include <glad/glad.h>
#include <stb_image.h>

#include "debug.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture 
{
public: 
	unsigned int ID; 
	int width, height, nrChannels;
	std::string type;
	Texture() : width(0), height(0), nrChannels(0) { glGenTextures(1, &ID); }
		
	void Generate(const char* texturePath, std::string textureType) 
	{
		type = textureType;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format;
			if (nrChannels == 1) format = GL_RED;
			if (nrChannels == 3) format = GL_RGB;
			if (nrChannels == 4) format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD at path: " << texturePath << std::endl;
		}
		stbi_image_free(data);
		glCheckError();
	}

	void Bind() { glBindTexture(GL_TEXTURE_2D, ID); }

};


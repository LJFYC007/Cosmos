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
		std::cout << "TEXTURE LOADING ::: Type ::: " << textureType << " ::: Path :::" << texturePath << " " << width << " " << height << " " << nrChannels << std::endl;
		if (type == "hdr")
		{
			stbi_set_flip_vertically_on_load(true);
			float* data = stbi_loadf(texturePath, &width, &height, &nrChannels, 0);
			if (data)
			{
				glBindTexture(GL_TEXTURE_2D, ID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

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
		}
		else if (type == "cubemap")
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
			for (unsigned int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else
		{ 
			stbi_set_flip_vertically_on_load(false);
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
		}
		glCheckError();
	}

	void Bind() 
	{
		if (type == "cubemap") glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 
		else glBindTexture(GL_TEXTURE_2D, ID); 
	}

};


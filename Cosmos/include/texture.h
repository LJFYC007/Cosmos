#pragma once
#include <glad/glad.h>
#include <stb_image.h>

#include "debug.h"

#include <vector>
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
		
	void Generate(std::string texturePath, std::string textureType) 
	{
		type = textureType;
		std::cout << "TEXTURE LOADING ::: Type ::: " << textureType << " ::: Path :::" << texturePath << " " << width << " " << height << " " << nrChannels << std::endl;
		if (type == "hdr")
		{
			stbi_set_flip_vertically_on_load(true);
			glBindTexture(GL_TEXTURE_2D, ID);
			float* data = stbi_loadf(texturePath.c_str(), &width, &height, &nrChannels, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
			if (data)
				stbi_image_free(data);
			else
				std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD at path: " << texturePath << std::endl;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_set_flip_vertically_on_load(false);
		}
		else if (type == "cubemap")
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
			if (texturePath == "")
			{
				for (unsigned int i = 0; i < 6; ++i)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			else
			{
				std::vector<std::string> faces{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
				for (unsigned int i = 0; i < 6; ++i)
				{ 
					unsigned char* data = stbi_load((texturePath + "-" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					if ( data ) stbi_image_free(data);
					else std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD at path: " << texturePath << std::endl;
				}
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else if (type == "brdf")
		{
			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{ 
			unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
			glBindTexture(GL_TEXTURE_2D, ID);

			GLenum format;
			if (nrChannels == 1) format = GL_RED;
			if (nrChannels == 3) format = GL_RGB;
			if (nrChannels == 4) format = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			if (data)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(data);
			}
			else
				std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD at path: " << texturePath << std::endl;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glCheckError();
	}

	void Bind() 
	{
		if (type == "cubemap") glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 
		else glBindTexture(GL_TEXTURE_2D, ID); 
	}

	void GenerateMipmap()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
};


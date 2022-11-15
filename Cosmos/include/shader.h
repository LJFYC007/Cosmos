#pragma once
#include <glad/glad.h>

#include "debug.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public : 
	unsigned int ID;

	Shader() { ID = 0; }
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// retrieve the vertex & fragment source code from filepath
		// -----------------------------------------------	
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << " " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// compile shaders
		// -----------------------------------------------	
		unsigned int vertex, fragment; 

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader program
		// -----------------------------------------------	
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// delete shaders
		// -----------------------------------------------	
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		glCheckError();
	}

	Shader& use() { glUseProgram(ID); return *this; }

	void setBool(const std::string &name, bool value) const 
	{ 
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string &name, int value) const 
	{ 
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string &name, float value) const 
	{ 
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMatrix(const std::string &name, float *value) const 
	{ 
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
	}

	void setMat4(const std::string &name, const glm::mat4& mat) const 
	{ 
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z) const 
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

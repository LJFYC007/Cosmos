#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

#include <vector>

class Quad
{
public: 
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Quad() { createQuad(); }

private:
    void createQuad() ;
};

void Quad::createQuad() {
	float vertices1[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	for (unsigned int i = 0; i < 4; ++i)
	{
		Vertex point;
		point.Position = glm::vec3(vertices1[5 * i], vertices1[5 * i + 1], vertices1[5 * i + 2]);
		point.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
		point.TexCoords = glm::vec2(vertices1[5 * i + 3], vertices1[5 * i + 4]);
		vertices.push_back(point);
	}
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);
}

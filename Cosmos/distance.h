#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/mesh.h"

#include <vector>

float fuckGG[64 * 64 * 64];

class Distance
{
public: 
	void SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices) ;
	float *QueryDistance();
private:
	int get(int i, int j, int k) { return i * 64 * 64 + j * 64 + k; }
};

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	float Unt = 0.2f / 63;
	for (int i = 0; i < 64; ++i) 
		for (int j = 0; j < 64; ++j) 
			for (int k = 0; k < 64; ++k) {
				fuckGG[get(i, j, k)] = 1.0f;

				glm::vec3 v(i * Unt - 0.1f, j * Unt - 0.1f, k * Unt - 0.1f);
				for (int t = 0;t < vertices.size();t ++) {
					glm::vec3 v0 = vertices[t].Position;
					glm::vec3 gg(v0.x - v.x, v0.y - v.y, v0.z - v.z);
					float distance = glm::length(gg);
					fuckGG[get(i, j, k)] = std::min(distance, fuckGG[get(i, j, k)]);
				}

				if (glm::length(v) < 0.1f)
					fuckGG[get(i, j, k)] = -fuckGG[get(i, j, k)];
			}
}

float *Distance::QueryDistance()
{
	return fuckGG;
}

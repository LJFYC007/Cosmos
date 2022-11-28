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
	float eps = 0.00001;
};

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	for (int i = 0; i < 64 * 64 * 64; ++i)
		fuckGG[i] = 1.0f;
	float Unt = 1.0 / 63;
	for (int i = 0;i < indices.size();i ++) {
		glm::vec3 v0 = vertices[indices[i]].Position;
		for(int i=0;i<64;++i)
			for(int j=0;j<64;++j)
				for (int k = 0;k < 64;++k) {
					glm::vec3 v = glm::vec3(i * Unt - 0.5f, j * Unt - 0.5f, k * Unt - 0.5f);
					float t = (v.x-v0.x)*(v.x-v0.x)+(v.y-v0.y)*(v.y-v0.y)+(v.z-v0.z)*(v.z-v0.z) ;
					fuckGG[i * 64 * 64 + j * 64 + k] = std::min(fuckGG[i * 64 * 64 + j * 64 + k], t);
				}
	}
	for (int i = 0;i < 64 * 64 * 64;++i)fuckGG[i] = sqrt(fuckGG[i]);
}

float *Distance::QueryDistance()
{
	return fuckGG;
}

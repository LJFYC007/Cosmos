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
		fuckGG[i] = 10.0f;
	float Unt = 2.0 / 63;
	for (int t = 0; t < indices.size(); t++) {
		glm::vec3 v0 = vertices[indices[t]].Position;
		for (int i = 0; i < 64; ++i)
			for (int j = 0; j < 64; ++j)
				for (int k = 0; k < 64; ++k) {
					glm::vec3 v = glm::vec3(i * Unt - 1.0f, j * Unt - 1.0f, k * Unt - 1.0f);
					float t = (v.x - v0.x) * (v.x - v0.x) + (v.y - v0.y) * (v.y - v0.y) + (v.z - v0.z) * (v.z - v0.z);
					fuckGG[i * 64 * 64 + j * 64 + k] = std::min(fuckGG[i * 64 * 64 + j * 64 + k], sqrt(t));
				}
	}
}

float *Distance::QueryDistance()
{
	return fuckGG;
}

/*
float fuckGG[64 * 64 * 64 * 3];

class Distance
{
public: 
	void SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices) ;
	float *QueryDistance();
private:
	float eps = 0.00001;
	int get(int i, int j, int k, int w) { return (i * 64 * 64 + j * 64 + k) * 3 + w; }
};

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	for (int i = 0; i < 64 * 64 * 64 * 3; ++i)
		fuckGG[i] = 10.0f;
	float Unt = 1.0 / 63;
	for (int t = 0;t < indices.size();t ++) {
		glm::vec3 v0 = vertices[indices[t]].Position;
		for(int i=0;i<64;++i)
			for(int j=0;j<64;++j)
				for (int k = 0;k < 64;++k) {
					glm::vec3 v = glm::vec3(i * Unt - 0.5f, j * Unt - 0.5f, k * Unt - 0.5f);
					glm::vec3 t(v.x - v0.x, v.y - v0.y, v.z - v0.z);
					if ( glm::length(t) < glm::length(glm::vec3(fuckGG[get(i, j, k, 0)], fuckGG[get(i, j, k, 1)], fuckGG[get(i, j, k, 2)])) )
					{ 
						fuckGG[get(i, j, k, 0)] = t.x;
						fuckGG[get(i, j, k, 1)] = t.y;
						fuckGG[get(i, j, k, 2)] = t.z;
					}
				}
	}
}

float *Distance::QueryDistance()
{
	return fuckGG;
}
*/

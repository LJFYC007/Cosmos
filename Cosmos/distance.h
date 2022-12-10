#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/mesh.h"

#include <vector>

float fuckGG[128 * 128 * 128 * 3];

class Distance
{
public: 
	void SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices) ;
	float *QueryDistance();
private:
	int get(int i, int j, int k, int w) { return (i * 128 * 128 + j * 128 + k) * 3 + w; }
	bool ray_triangle_intersect_mt(glm::vec3 O, glm::vec3 D, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
};

bool Distance::ray_triangle_intersect_mt(glm::vec3 O, glm::vec3 D, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
	glm::vec3 E1 = p1 - p0, E2 = p2 - p0, S = O - p0;
	glm::vec3 S1 = glm::cross(D, E2), S2 = glm::cross(S, E1);
	float coeff = 1.0 / glm::dot(S1, E1);
	float t = coeff * glm::dot(S2, E2);
	float u = coeff * glm::dot(S1, S);
	float v = coeff * glm::dot(S2, D);
	if (t >= 0 && u >= 0 && v >= 0 && (1 - u - v) >= 0)
		return true;
	return false;
}

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	float Unt = 0.2f / 127;
	for (int i = 0; i < 128; ++i) 
		for (int j = 0; j < 128; ++j) 
			for (int k = 0; k < 128; ++k) {
				fuckGG[get(i, j, k, 0)] = 100.0f;
				fuckGG[get(i, j, k, 1)] = 100.0f;
				fuckGG[get(i, j, k, 2)] = 100.0f;
				glm::vec3 v(i * Unt - 0.1f, j * Unt - 0.1f, k * Unt - 0.1f);
				for (int t = 0; t < vertices.size(); t++) {
					glm::vec3 v0 = vertices[t].Position;
					glm::vec3 gg(v0.x - v.x, v0.y - v.y, v0.z - v.z);
					float distance = glm::length(gg);
					if (distance < glm::length(glm::vec3(fuckGG[get(i, j, k, 0)], fuckGG[get(i, j, k, 1)], fuckGG[get(i, j, k, 2)])))
					{
						fuckGG[get(i, j, k, 0)] = v.x - v0.x;
						fuckGG[get(i, j, k, 1)] = v.y - v0.y;
						fuckGG[get(i, j, k, 2)] = v.z - v0.z;
					}
				}
			}

	srand(20040515);
	float x = (float)rand() / RAND_MAX;
	float y = (float)rand() / RAND_MAX;
	float z = (float)rand() / RAND_MAX;
	glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));

	for (int i = 0; i < 128; ++i) 
		for (int j = 0; j < 128; ++j) 
			for (int k = 0; k < 128; ++k) {
				glm::vec3 v(i * Unt - 0.1f, j * Unt - 0.1f, k * Unt - 0.1f);
				int cross_cnt = 0;
				for (int id = 0; id < indices.size(); id += 3)
					if (ray_triangle_intersect_mt(v, dir, vertices[indices[id]].Position, vertices[indices[id + 1]].Position, vertices[indices[id + 2]].Position))
						++cross_cnt;
				if (cross_cnt & 1) fuckGG[get(i, j, k, 0)] = fuckGG[get(i, j, k, 1)] = fuckGG[get(i, j, k, 2)] =  -1000000.0f;
			}
}

float *Distance::QueryDistance()
{
	return fuckGG;
}

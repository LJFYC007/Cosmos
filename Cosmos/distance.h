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
	for (int i = 0;i < indices.size();i += 3) {
		glm::vec3 v0 = vertices[indices[i]].Position, v1 = vertices[indices[i + 1]].Position, v2 = vertices[indices[i + 2]].Position;
		float a = (v1.y - v0.y) * (v2.z - v0.z) - (v2.y - v0.y) * (v1.z - v0.z);
		float b = (v2.x - v0.x) * (v1.z - v0.z) - (v1.x - v0.x) * (v2.z - v0.z);
		float c = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
		glm::vec3 Normal = glm::vec3(a, b, c);
		float x0 = v0.x, y0 = v0.y, z0 = v0.z;
		float A2B2C2 = a * a + b * b + c * c,abc=a*x0+b*y0+c*z0,sq=glm::sqrt(A2B2C2);
		for(int i=0;i<64;++i)
			for(int j=0;j<64;++j)
				for (int k = 0;k < 64;++k) {
					glm::vec3 v = glm::vec3(i * Unt - 0.5f, j * Unt - 0.5f, k * Unt - 0.5f);
					float t = (abc - a * v.x - b * v.y - c * v.z) / A2B2C2;
					fuckGG[i * 64 * 64 + j * 64 + k] = std::min(fuckGG[i * 64 * 64 + j * 64 + k], abs(t * sq));
				}
	}
	std::cerr << fuckGG[32 * 64 * 64 + 32 * 64 + 32] << std::endl;
	std::cerr << fuckGG[0 * 64 * 64 + 32 * 64 + 32] << std::endl;
}

float *Distance::QueryDistance()
{
	return fuckGG;
}

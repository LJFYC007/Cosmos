#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/mesh.h"

#include <vector>

class Distance
{
	// 保证顶点坐标在[-0.5f,0.5f]范围内，查询点坐标也在[-2.0f,2.0f]范围内
	// 大概传进来20000个三角形的vector
	// struct Vertex { glm::vec3 Position, Normal; glm::vec2 TexCoords; };
	// 上面是 Vertex 的定义函数，如果需要用法线直接调用就行
public: 
	void SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices) ;

	float *QueryDistance();
private:
	float fuckGG[64 * 64 * 64];
	const float eps = 1e-5;
};

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	float Unt = 1.0 / 63;
	for (int i = 0;i < indices.size();i += 3) {
		glm::vec3 v0 = vertices[indices[i]].Position, v1 = vertices[indices[i + 1]].Position, v2 = vertices[indices[i + 2]].Position;
		double a = (v1.y - v0.y) * (v2.z - v0.z) - (v2.y - v0.y) * (v1.z - v0.z);
		double b = (v2.x - v0.x) * (v1.z - v0.z) - (v1.x - v0.x) * (v2.z - v0.z);
		double c = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
		glm::vec3 Normal = glm::vec3(a, b, c);
		double x0 = v0.x, y0 = v0.y, z0 = v0.z;
		double A2B2C2 = a * a + b * b + c * c,abc=a*x0+b*y0+c*z0,sq=glm::sqrt(A2B2C2);
		for(int i=0;i<64;++i)
			for(int j=0;j<64;++j)
				for (int k = 0;k < 64;++k) {
					glm::vec3 v = glm::vec3(i * Unt, j * Unt, k * Unt);
					double t = (abc - a * v.x - b * v.y - c * v.z) / A2B2C2;
					fuckGG[i * 64 * 64 + j * 64 + k] = t * sq;
				}
	}
	return fuckGG;
}

float *Distance::QueryDistance()
{
	return fuckgg;
}

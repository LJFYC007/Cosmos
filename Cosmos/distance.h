#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/mesh.h"

#include <vector>

class Distance
{
	// 保证顶点坐标在[-2.0f,2.0f]范围内，查询点坐标也在[-2.0f,2.0f]范围内
	// 大概传进来20000个三角形的vector
	// struct Vertex { glm::vec3 Position, Normal; glm::vec2 TexCoords; };
	// 上面是 Vertex 的定义函数，如果需要用法线直接调用就行
public: 
	void SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices) ;

	float QueryDistance(glm::vec3 point);
private: 
};

void Distance::SetupDistanceField(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{

}

float Distance::QueryDistance(glm::vec3 point)
{
	return 0.0f;
}

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/mesh.h"

#include <vector>


class Distance2
{
public:
	float QueryDistance(glm::vec3 v);
private:
	float eps = 0.00001;
};


float Distance2::QueryDistance(glm::vec3 v)
{
	bool inx = -0.1 <= v.x && v.x <= 0.1;
	bool iny = -0.1 <= v.y && v.y <= 0.1;
	bool inz = -0.1 <= v.z && v.z <= 0.1;
	float dx = 0, dy = 0, dz = 0,ans=100;
	for(int i=0;i<=1;++i)
		for(int j=0;j<=1;++j)
			for (int k = 0;k <= 1;++k) {
				float x = i ? -0.1 : 0.1, y = j ? -0.1 : 0.1, z = k ? -0.1 : 0.1;
				if (!inx) dx = (v.x - x) * (v.x - x);
				if (!iny) dy = (v.y - y) * (v.y - y);
				if (!inz) dz = (v.z - z) * (v.z - z);
				ans = min(ans, dx + dy + dz);
			}
	return sqrt(ans); 
}

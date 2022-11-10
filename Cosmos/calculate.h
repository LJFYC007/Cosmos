#pragma once
#include "include/resource.h"
#include "include/sphere_render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Calculate
{
public: 
	static void Render(SphereRenderer* sphere);

private:	
};

void Calculate::Render(SphereRenderer* sphere)
{
	sphere->Draw(Resource::GetTexture("earth"), glm::vec3(1.0f, 1.0f, 1.0f));
	//sphere->Draw(Resource::GetTexture("sun"), glm::vec3(10.0f, 0.0f, 3.0f));
}
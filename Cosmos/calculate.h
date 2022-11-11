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
	sphere->Draw(Resource::GetTexture("earth"), glm::vec3(0.0f, 0.0f, 0.0f));
	sphere->Draw(Resource::GetTexture("sun"), glm::vec3(2.0f, 5.0f, -15.0f));
}
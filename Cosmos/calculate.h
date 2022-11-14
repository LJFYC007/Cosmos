#pragma once
#include "include/resource.h"
#include "earth_render.h"
#include "sun_render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Calculate
{
public: 
	static void Render(SunRenderer& sun, EarthRenderer& earth);

private: 
};

void Calculate::Render(SunRenderer& sun, EarthRenderer& earth)
{
	earth.Draw(glm::vec3(0.0f, 0.0f, 0.0f));
	sun.Draw(Resource::GetTexture("sun"), glm::vec3(2.0f,1.0f, -1.0f));
}
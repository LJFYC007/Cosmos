#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "earth_render.h"
#include "sun_render.h"
#include "moon_render.h"

class Calculate
{
public: 
	static void Render(SunRenderer& sun, EarthRenderer& earth, MoonRenderer& moon);

private: 
};

void Calculate::Render(SunRenderer& sun, EarthRenderer& earth, MoonRenderer& moon)
{
	earth.Draw(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f);
	moon.Draw(glm::vec3(-0.5f, 0.3f, 1.5f), 0.273f);
	// sun.Draw(glm::vec3(2.0f,1.0f, -1.0f));
}
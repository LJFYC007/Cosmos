#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/render.h"
#include "include/shader.h"
#include "include/mesh.h"

class SunRenderer : public Renderer
{
public : 
    SunRenderer(Shader& otherShader, Mesh& otherMesh) : Renderer(otherShader, otherMesh) {};
};


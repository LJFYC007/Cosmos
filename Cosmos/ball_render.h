#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/render.h"
#include "include/shader.h"
#include "include/mesh.h"
#include "include/camera.h"

class BallRenderer : public Renderer
{
public : 
    BallRenderer(Shader& otherShader, Mesh& otherMesh) : Renderer(otherShader, otherMesh) {};
    void Draw(glm::vec3 position, GLfloat size = 1.0f) override;
};

void BallRenderer::Draw(glm::vec3 position, GLfloat size)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);

    shader.setVec3("Albedo", 1.0f, 0.843f, 0.0f);
    shader.setFloat("metallic", 0.4f);
	shader.setFloat("roughness", 0.5f);
	shader.setFloat("ao", 1.0f);

    mesh.Draw(shader);
}

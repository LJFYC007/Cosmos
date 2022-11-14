#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/render.h"
#include "include/shader.h"
#include "include/mesh.h"
#include "include/camera.h"

class MoonRenderer : public Renderer
{
public : 
    MoonRenderer(Shader& otherShader, Mesh& otherMesh) : Renderer(otherShader, otherMesh) {};
    void Draw(glm::vec3 position, GLfloat size = 1.0f) override;
};

void MoonRenderer::Draw(glm::vec3 position, GLfloat size)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);

    shader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.014f);
    shader.setFloat("pointLights[0].quadratic", 0.0007f);

    mesh.Draw(shader);
}

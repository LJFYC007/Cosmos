#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/render.h"
#include "include/shader.h"
#include "include/mesh.h"
#include "include/camera.h"

class EarthRenderer : public Renderer
{
public : 
    EarthRenderer(Shader& otherShader, Mesh& otherMesh) : Renderer(otherShader, otherMesh) {};
    void Draw(glm::vec3 position, GLfloat size = 1.0f) override;
};

void EarthRenderer::Draw(glm::vec3 position, GLfloat size)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);

    shader.setVec3("pointLights[0].ambient", 1.0f, 1.0f, 1.0f);
    shader.setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("pointLights[0].specular", 0.1f, 0.1f, 0.1f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.014f);
    shader.setFloat("pointLights[0].quadratic", 0.0007f);
    shader.setFloat("material.shininess", 32.0f);
    shader.setFloat("cloudtrans", 0.7f);

    mesh.Draw(shader);
}

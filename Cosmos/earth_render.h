#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.h"
#include "include/mesh.h"

class EarthRenderer
{
public : 
    EarthRenderer(Shader& otherShader, Mesh& otherMesh);
    void Draw(glm::vec3 position, GLfloat size = 1.0f);

private : 
    Shader shader;
    Mesh mesh;
};

EarthRenderer::EarthRenderer(Shader& otherShader, Mesh& otherMesh) { shader = otherShader; mesh = otherMesh; }

void EarthRenderer::Draw(glm::vec3 position, GLfloat size)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);

    shader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
    shader.setVec3("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.014f);
    shader.setFloat("pointLights[0].quadratic", 0.0007f);
    shader.setFloat("material.shininess", 4.0f);

    mesh.Draw(shader);
}

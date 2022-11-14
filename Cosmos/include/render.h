#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"
#include "camera.h"

class Renderer
{
public : 
    Renderer(Shader& otherShader, Mesh& otherMesh);
    virtual void Draw(glm::vec3 position, GLfloat size = 1.0f);

protected : 
    Shader shader; Mesh mesh;
};

Renderer::Renderer(Shader& otherShader, Mesh& otherMesh) { shader = otherShader; mesh = otherMesh; }

void Renderer::Draw(glm::vec3 position, GLfloat size)
{
    shader.use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);
    mesh.Draw(shader);
}

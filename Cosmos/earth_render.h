#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/texture.h"
#include "include/shader.h"

class EarthRenderer
{
public : 
    EarthRenderer(Shader& shader);
    ~EarthRenderer();
    void Draw(Texture texture1, Texture texture2, Texture texture3, glm::vec3 position, GLfloat size = 1.0f);

private : 
	const GLuint lats = 30;
	const GLuint lons = 30;
	const GLfloat r = 0.5f;
    GLfloat vertices[14 * 6 * 30 * 30]; 

    Shader shader;
    GLuint VAO;
    
    void initRenderData();
    void createSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude);
    glm::vec3 getPoint(GLfloat u, GLfloat v);
    glm::vec2 getTexCoord1(GLfloat u, GLfloat v) { return glm::vec2(v, 1 - u); }
    glm::vec3 getTexCoord2(GLfloat u, GLfloat v) { return glm::vec3(cos(u) * sin(v), cos(u) * cos(v), -sin(u)); }
    glm::vec3 getTexCoord3(GLfloat u, GLfloat v) { return glm::vec3(sin(u) * cos(v), -cos(u) * sin(v), 0); }
    void insertArray(GLfloat* sphere, GLuint& offset, const GLfloat* array, GLuint n)
    {
        memcpy(sphere + offset, array, n * sizeof(GLfloat));
        offset += n;
    }
};

EarthRenderer::EarthRenderer(Shader& other)
{
    shader = other;
    shader.use(); 
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setInt("material.normal", 2);
    initRenderData();
}

EarthRenderer::~EarthRenderer() { glDeleteVertexArrays(1, &VAO); }

void EarthRenderer::Draw(Texture texture1, Texture texture2, Texture texture3, glm::vec3 position, GLfloat size)
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

    glActiveTexture(GL_TEXTURE0);
    texture1.Bind();
    glActiveTexture(GL_TEXTURE1);
    texture2.Bind();
    glActiveTexture(GL_TEXTURE2);
    texture3.Bind();
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * lats * lons);
    glBindVertexArray(0);
}

void EarthRenderer::initRenderData()
{
    GLuint VBO;
	createSphere(vertices, lons, lats);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec3 EarthRenderer::getPoint(GLfloat u, GLfloat v) {
	GLfloat pi = glm::pi<GLfloat>();
	GLfloat z = r * std::cos(pi * u);
	GLfloat x = r * std::sin(pi * u) * std::cos(2 * pi * v);
	GLfloat y = r * std::sin(pi * u) * std::sin(2 * pi * v);
    return glm::vec3(x, y, z);
}

void EarthRenderer::createSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude) {
  GLfloat lon_step = 1.0f / Longitude;
  GLfloat lat_step = 1.0f / Latitude;
  GLuint offset = 0;
  for (unsigned int lat = 0; lat < Latitude; lat++) {
    for (unsigned int lon = 0; lon < Longitude; lon++) {
      glm::vec3 point1 = getPoint(lat * lat_step, lon * lon_step);
      glm::vec3 point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
      glm::vec3 point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
      glm::vec3 point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);

      insertArray(sphere, offset, glm::value_ptr(point1), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1(lat * lat_step, lon * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2(lat * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3(lat * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point1), 3);
      insertArray(sphere, offset, glm::value_ptr(point4), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1(lat * lat_step, (lon + 1) * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2(lat * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3(lat * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point4), 3);
      insertArray(sphere, offset, glm::value_ptr(point3), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1((lat + 1) * lat_step, (lon + 1) * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2((lat + 1) * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3((lat + 1) * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point3), 3);

      insertArray(sphere, offset, glm::value_ptr(point1), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1(lat * lat_step, lon * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2(lat * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3(lat * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point1), 3);
      insertArray(sphere, offset, glm::value_ptr(point3), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1((lat + 1) * lat_step, (lon + 1) * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2((lat + 1) * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3((lat + 1) * lat_step, (lon + 1) * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point3), 3);
      insertArray(sphere, offset, glm::value_ptr(point2), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord1((lat + 1) * lat_step, lon * lon_step)), 2);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord2((lat + 1) * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(getTexCoord3((lat + 1) * lat_step, lon * lon_step)), 3);
      insertArray(sphere, offset, glm::value_ptr(point2), 3);
    }
  }
}

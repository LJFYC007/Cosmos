#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/texture.h"
#include "include/shader.h"

const GLuint lats = 100;
const GLuint lons = 100;

class SphereRenderer
{
public : 
    SphereRenderer(Shader& shader);
    ~SphereRenderer();
    void Draw(Texture texture, glm::vec3 position, GLfloat size);

private : 
    Shader shader;
    GLuint VAO;
    void initRenderData();
    void createSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude);
    glm::vec3 getPoint(GLfloat u, GLfloat v);
};

SphereRenderer::SphereRenderer(Shader& other)
{
    shader = other;
    initRenderData();
}

SphereRenderer::~SphereRenderer() { glDeleteVertexArrays(1, &VAO); }

void SphereRenderer::Draw(Texture texture, glm::vec3 position, GLfloat size)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(size));
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    //texture.Bind();
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * lats * lons);
    glBindVertexArray(0);
}

void SphereRenderer::initRenderData()
{
    GLuint VBO;
	GLfloat vertices[6 * 3 * lats * lons]; 
	createSphere(vertices, lons, lats);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec3 SphereRenderer::getPoint(GLfloat u, GLfloat v) {
    GLfloat r = 0.5f;
	GLfloat pi = glm::pi<GLfloat>();
	GLfloat z = r * std::cos(pi * u);
	GLfloat x = r * std::sin(pi * u) * std::cos(2 * pi * v);
	GLfloat y = r * std::sin(pi * u) * std::sin(2 * pi * v);
    return glm::vec3(x, y, z);
}

void SphereRenderer::createSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude) {
  GLfloat lon_step = 1.0f / Longitude;
  GLfloat lat_step = 1.0f / Latitude;
  GLuint offset = 0;
  for (unsigned int lat = 0; lat < Latitude; lat++) {
    for (unsigned int lon = 0; lon < Longitude; lon++) {
      glm::vec3 point1 = getPoint(lat * lat_step, lon * lon_step);
      glm::vec3 point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
      glm::vec3 point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
      glm::vec3 point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);
      memcpy(sphere + offset, glm::value_ptr(point1), 3 * sizeof(GLfloat));
      offset += 3;
      memcpy(sphere + offset, glm::value_ptr(point4), 3 * sizeof(GLfloat));
      offset += 3;
      memcpy(sphere + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
      offset += 3;

      memcpy(sphere + offset, glm::value_ptr(point1), 3 * sizeof(GLfloat));
      offset += 3;
      memcpy(sphere + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
      offset += 3;
      memcpy(sphere + offset, glm::value_ptr(point2), 3 * sizeof(GLfloat));
      offset += 3;
    }
  }
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <assimp/config.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.h"
#include "include/camera.h"

#include <iostream>

#define REP(i, a, b) for ( int i = a, _end_ = b; i <= _end_; ++ i )

// settings
const int screenWidth = 2000;
const int screenHeight = 1500;
const GLuint lats = 100;
const GLuint lons = 100;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2;
float lastY = screenHeight / 2;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastTime = 0.0f;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	// WASD movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos; lastY = ypos;
		firstMouse = false;
	}
	camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);
	lastX = xpos; lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const *path)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		if (nrChannels == 3) format = GL_RGB;
		if (nrChannels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD at path: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureId;
}

glm::vec3 getPoint(GLfloat u, GLfloat v) {
	GLfloat r = 0.5f;
	GLfloat pi = glm::pi<GLfloat>();
	GLfloat z = r * std::cos(pi * u);
	GLfloat x = r * std::sin(pi * u) * std::cos(2 * pi * v);
	GLfloat y = r * std::sin(pi * u) * std::sin(2 * pi * v);
    return glm::vec3(x, y, z);
}

void createSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude) {
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


int main()
{
	// Initialize
	// -----------------------------------------------	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------------------------	
	glEnable(GL_DEPTH_TEST);

	// build and compile shader program
	// -----------------------------------------------	
	Shader lightingShader("color.vs", "color.fs");

	// set up vertex data
	// -----------------------------------------------	
	GLfloat vertices[6 * 3 * lats * lons]; 
	createSphere(vertices, lons, lats);

	// configure the cube's VAO and VBO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// load & create textures
	// -----------------------------------------------	
	unsigned int textureMap = loadTexture("resources/textures/awesomeface.png");
	lightingShader.use();
	lightingShader.setInt("aTexture", 0);

	// render loop
	// -----------------------------------------------	
	while (!glfwWindowShouldClose(window))
	{
		// pre-frame time logic	
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Input
		processInput(window); 

		// Render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // alpha : 不透明度
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();

		// cube transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMap);

		// cube render
		glBindVertexArray(cubeVAO);
		REP(i, 0, 9)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6 * lats * lons);
		}

		// Swap Buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

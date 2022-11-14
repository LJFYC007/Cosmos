#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/camera.h"
#include "include/resource.h"
#include "include/texture.h"
#include "include/sphere.h"
#include "earth_render.h"
#include "sun_render.h"
#include "calculate.h"

#include <iostream>
#include <vector>

#define REP(i, a, b) for ( int i = a, _end_ = b; i <= _end_; ++ i )

// settings
const int screenWidth = 2000;
const int screenHeight = 1500;

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

	// load shader & textures & meshes
	// -----------------------------------------------	

	// ------------------ earth ----------------------
	Resource::LoadShader("earth.vs", "earth.fs", "earth");
	std::vector<Texture> earthTextures;
	earthTextures.push_back(Resource::LoadTexture("resources/textures/2k_earth_daymap.jpg", "earth_diffuse", "diffuse"));
	earthTextures.push_back(Resource::LoadTexture("resources/textures/2k_earth_specular_map.jpg", "earth_specular", "specular"));
	earthTextures.push_back(Resource::LoadTexture("resources/textures/2k_earth_normal_map.png", "earth_normal", "normal"));
	Sphere sphere;
	Resource::LoadMesh(sphere.vertices, sphere.indices, earthTextures, "earth");
	EarthRenderer earth(Resource::GetShader("earth"), Resource::GetMesh("earth"));

	// ------------------- sun ------------------------
	Resource::LoadShader("sun.vs", "sun.fs", "sun");
	std::vector<Texture> sunTextures;
	sunTextures.push_back(Resource::LoadTexture("resources/textures/sun.jpg", "sun_diffuse", "diffuse"));
	Resource::LoadMesh(sphere.vertices, sphere.indices, sunTextures, "sun");
	SunRenderer sun(Resource::GetShader("sun"), Resource::GetMesh("sun"));

	std::cout << "Finish Initialize ---------- Start Rendering" << std::endl;

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

		Resource::GetShader("earth").use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		Resource::GetShader("earth").setMat4("projection", projection);
		Resource::GetShader("earth").setMat4("view", view);
		Resource::GetShader("earth").setVec3("viewPos", camera.Position);
		Resource::GetShader("earth").setVec3("lightPos", glm::vec3(2.0f, 1.0f, -8.0f));

		Resource::GetShader("sun").use();
		Resource::GetShader("sun").setMat4("projection", projection);
		Resource::GetShader("sun").setMat4("view", view);

		Calculate::Render(sun, earth);

		// Swap Buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
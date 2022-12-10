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
#include "include/quad.h"
#include "include/cube.h"
#include "include/debug.h"
#include "include/model.h"
#include "ball_render.h"
#include "calculate.h"
#include "distance.h"

#include <iostream>
#include <vector>

#define REP(i, a, b) for ( int i = a, _end_ = b; i <= _end_; ++ i )

// window settings
const int screenWidth = 2000;
const int screenHeight = 1500;

// camera
Camera camera(glm::vec3(0.41f, 0.62f, -0.14f), glm::vec3(0.0f, 1.0f, 0.0f), 171.6f, -55.7f);
float lastX = screenWidth / 2;
float lastY = screenHeight / 2;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastTime = 0.0f;
float beginTime = 0.0f;
int fpsFrames = 0;

// standard objects
Sphere sphere;
Cube cube;
Quad quad;

// frame settings
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void mouse_callback(GLFWwindow * window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

int main()
{
	// Initialize
	// -----------------------------------------------	
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
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
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_MULTISAMPLE);

	glCheckError();

	// PBR skybox setup
	// -----------------------------------------------	
	// some initialize
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// start render hdr map
	Resource::LoadTexture("", "envCubemap", "cubemap");
	std::vector<Texture> hdrTextures;
	hdrTextures.push_back(Resource::LoadTexture("resources/hdr/lake.hdr", "hdrTexture", "hdr"));
	Resource::LoadShader("hdr.vs", "hdr.fs", "hdr");
	Resource::LoadMesh(cube.vertices, cube.indices, hdrTextures, "hdr");

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		Resource::GetShader("hdr").use();
		Resource::GetShader("hdr").setMat4("view", captureViews[i]);
		Resource::GetShader("hdr").setMat4("projection", captureProjection);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Resource::GetTexture("envCubemap").ID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Resource::GetMesh("hdr").Draw(Resource::GetShader("hdr"));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Resource::GetTexture("envCubemap").GenerateMipmap();
	// Resource::LoadTexture("resources/cubemap/cubemap", "envCubemap", "cubemap");

	// start render irradiance map
	Resource::LoadTexture("", "irrCubemap", "cubemap");
	std::vector<Texture> envCubemaps;
	envCubemaps.push_back(Resource::GetTexture("envCubemap"));
	Resource::LoadShader("irr.vs", "irr.fs", "irr");
	Resource::LoadMesh(cube.vertices, cube.indices, envCubemaps, "irr");

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		Resource::GetShader("irr").use();
		Resource::GetShader("irr").setMat4("projection", captureProjection);
		Resource::GetShader("irr").setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Resource::GetTexture("irrCubemap").ID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Resource::GetMesh("irr").Draw(Resource::GetShader("irr"));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Resource::GetTexture("irrCubemap").GenerateMipmap();

	// start render prefilter map
	Resource::LoadTexture("", "prefilterCubemap", "cubemap");
	Resource::GetTexture("prefilterCubemap").GenerateMipmap();
	Resource::LoadShader("prefilter.vs", "prefilter.fs", "prefilter");
	Resource::LoadMesh(cube.vertices, cube.indices, envCubemaps, "prefilter");

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 9;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		unsigned int mipWidth = static_cast<unsigned int>(512 * std::pow(0.5, mip));
		unsigned int mipHeight = static_cast<unsigned int>(512 * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		Resource::GetShader("prefilter").use();
		Resource::GetShader("prefilter").setMat4("projection", captureProjection);
		Resource::GetShader("prefilter").setFloat("roughness", roughness);

		for (unsigned int i = 0; i < 6; ++i)
		{
			Resource::GetShader("prefilter").setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Resource::GetTexture("prefilterCubemap").ID, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Resource::GetMesh("prefilter").Draw(Resource::GetShader("prefilter"));
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// start render brdf map
	Resource::LoadTexture("", "brdfTexture", "brdf");
	Resource::LoadShader("brdf.vs", "brdf.fs", "brdf");
	std::vector<Texture> emptyTextures;
	Resource::LoadMesh(quad.vertices, quad.indices, emptyTextures, "brdf");

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Resource::GetTexture("brdfTexture").ID, 0);

	glViewport(0, 0, 512, 512);
	Resource::GetShader("brdf").use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Resource::GetMesh("brdf").Draw(Resource::GetShader("brdf"));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// load shader & textures & meshes
	// -----------------------------------------------	

	// ------------------ ball -----------------------
	Resource::LoadShader("ball.vs", "ball.fs", "ball");
	std::vector<Texture> ballTextures;
	ballTextures.push_back(Resource::LoadTexture("resources/textures/earth_normal_map.png", "earth_normal", "normal"));
	ballTextures.push_back(Resource::GetTexture("irrCubemap"));
	ballTextures.push_back(Resource::GetTexture("prefilterCubemap"));
	ballTextures.push_back(Resource::GetTexture("brdfTexture"));
	Resource::LoadMesh(sphere.vertices, sphere.indices, ballTextures, "ball");
	BallRenderer ball(Resource::GetShader("ball"), Resource::GetMesh("ball"));

	Resource::LoadShader("background.vs", "background.fs", "background");
	Resource::LoadMesh(cube.vertices, cube.indices, envCubemaps, "background");

	Resource::LoadShader("backquad.vs", "backquad.fs", "backquad");
	std::vector<Texture> quadMap;
	quadMap.push_back(Resource::LoadTexture("resources/cubemap/cubemap_bottom.jpg", "bottom", "diffuse"));
	Resource::LoadMesh(quad.vertices, quad.indices, quadMap, "backquad");

	// --------------- distance field ----------------
	Distance distance;
	distance.SetupDistanceField(sphere.vertices, sphere.indices);
	GLfloat* distanceData = distance.QueryDistance();

	unsigned int distanceField;
	glGenTextures(1, &distanceField);
	glBindTexture(GL_TEXTURE_3D, distanceField);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, 64, 64, 64, 0, GL_RED, GL_FLOAT, distanceData);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_3D, 0);

	// finish initialize
	// -----------------------------------------------	
	glViewport(0, 0, screenWidth, screenHeight);
	glCheckError();
	std::cout << "Finish Initialize ---------- Start Rendering" << std::endl;

	// Model Hahaha("resources/ball.obj");

	// render loop
	// -----------------------------------------------	
	while (!glfwWindowShouldClose(window))
	{
		// pre-frame time logic	
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		++fpsFrames;
		if (currentTime - beginTime > 1)
		{
			std::cout << "FPS : " << fpsFrames << std::endl;
			beginTime = currentTime;
			fpsFrames = 0;
		}

		// Input
		processInput(window);

		// Render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // alpha : transparancy
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		Resource::GetShader("ball").use();
		Resource::GetShader("ball").setMat4("projection", projection);
		Resource::GetShader("ball").setMat4("view", view);
		Resource::GetShader("ball").setVec3("viewPos", camera.Position);
		Resource::GetShader("ball").setVec3("lightPos", glm::vec3(0.3f, 0.3f, 0.3f));

		ball.Draw(glm::vec3(0.0f, 0.0f, 0.0f));

		Resource::GetShader("backquad").use();
		Resource::GetShader("backquad").setMat4("view", view);
		Resource::GetShader("backquad").setMat4("projection", projection);
		Resource::GetShader("backquad").setVec3("lightPos", glm::vec3(0.3f, 0.3f, 0.3f));
		glm::mat4 model;

		model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f));
		model = glm::scale(model, glm::vec3(2.0f));
		Resource::GetShader("backquad").setMat4("model", model);

		glActiveTexture(GL_TEXTURE1);
		Resource::GetShader("backquad").setInt("map", 1);
		glBindTexture(GL_TEXTURE_3D, distanceField);

		Resource::GetMesh("backquad").Draw(Resource::GetShader("backquad"));

		// Hahaha.Draw(Resource::GetShader("backquad"));

		// Swap Buffer
		glfwSwapBuffers(window);
		glfwPollEvents();

		glCheckError();
	}

	glfwTerminate();
	return 0;
}

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


#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr char TITLE[] = "MeshShader Sample";

bool InitializeGLFW()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

GLFWwindow* CreateWindow(int width, int height, const char* title)
{
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	return window;
}

bool InitializeGLAD() {
	int version = gladLoadGL(glfwGetProcAddress);
	if (!version) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

int main()
{
	if(!InitializeGLFW()) return -1;
	
	GLFWwindow* window = CreateWindow(WIDTH, HEIGHT, TITLE);
	if (!window) return -1;

	glfwMakeContextCurrent(window);

	if(!InitializeGLAD()) return -1;

	{
		Shader shader("../shader/triangle.mesh", "../shader/triangle.frag", ShaderType::MESH, ShaderType::FRAGMENT);

		// Loop 
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT);

			// Draw
			shader.Use();
			glDrawMeshTasksNV(0, 1);

			glfwSwapBuffers(window);
		}
	}

	glfwTerminate();
	return 0;
}
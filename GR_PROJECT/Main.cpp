#include "RendererD3D11.hpp"

#include <memory>

#include <GLFW/glfw3.h>

int main(void) {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "G-Render", nullptr, nullptr);
	if (window == nullptr)
		return -1;

	Renderer* renderer = new RendererD3D11(window);

	renderer->Init();

	glfwShowWindow(window);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		renderer->Draw();
	}

	glfwHideWindow(window);

	renderer->Shutdown();

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
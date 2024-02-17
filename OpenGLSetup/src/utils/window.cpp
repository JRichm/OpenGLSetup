#include "window.h"

// initialize GLFW, create window, etc
Window::Window(const char* title, int width, int height) {
	glfwInit();
	glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!glfwWindow) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Set the GLFW window as a user pointer
	glfwSetWindowUserPointer(glfwWindow, this);
}

// Cleanup GLFW, destroy window, etc
Window::~Window() {
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

GLFWwindow* Window::getGLFWWindow() const {
	return glfwWindow;
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(glfwWindow);
}

void Window::clear() {
	glClearColor(0.0f, 0.04f, 0.08f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers() {
	glfwSwapBuffers(glfwWindow);
}

void Window::pollEvents() {
	glfwPollEvents();
}
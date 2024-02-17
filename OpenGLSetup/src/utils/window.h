#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
	public:
		Window(const char* title, int width, int height);
		~Window();

		GLFWwindow* getGLFWWindow() const;

		bool shouldClose() const;
		void clear();
		void swapBuffers();
		void pollEvents();

	private:
		GLFWwindow* glfwWindow;
};

#pragma once
#include "window.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>

class Input {
	public:
		Input(const Window& window, Shader& shader);

		void processInput();
		const glm::mat4& getViewMatrix() const;
		void drawGraph();

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	private:
		unsigned int VAO;
		double lastX;
		double lastY;
		double initialX;
		double initialY;
		bool mousePressed;
		glm::mat4 viewMatrix;
		Shader& shader;

};
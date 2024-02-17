#include "input.h"

// initialize input, e.g., set up callbacks
Input::Input(const Window& window, Shader& shader) : shader(shader), lastX(0.0), lastY(0.0), initialX(0.0), initialY(0.0), mousePressed(false) {
    glfwSetMouseButtonCallback(window.getGLFWWindow(), mouse_button_callback);
    glfwSetCursorPosCallback(window.getGLFWWindow(), cursor_position_callback);
    lastX = 0.0;
    lastY = 0.0;
    initialX = 0.0;
    initialY = 0.0;
    mousePressed = false;

    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);

        // Create vertex buffer object with vertices data
        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        float vertices[] = {
            // line graph
            -0.9f, -0.5f, 0.0f, // Point 1
            -0.6f,  0.3f, 0.0f, // Point 2
            -0.3f, -0.7f, 0.0f, // Point 3
             0.0f,  0.5f, 0.0f, // Point 4
             0.3f, -0.2f, 0.0f, // Point 5
             0.6f,  0.8f, 0.0f, // Point 6

             // x-axis
            -1.0f,  0.0f, 0.0f,
             1.0f,  0.0f, 0.0f,

             // y-axis
              0.0f, -1.0f, 0.0f,
              0.0f,  1.0f, 0.0f
        };

        unsigned int indices[] = {
            // line graph
            0, 1, 2, 3, 4, 5,

            // x-axis
            6, 7,

            // y-axis
            8, 9
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Input::processInput() {
	// Process input, e.g., handle keyboard input
}

const glm::mat4& Input::getViewMatrix() const {
	return viewMatrix;
}

void Input::drawGraph() {
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(8 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* inputInstance = static_cast<Input*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            inputInstance->mousePressed = true;
            glfwGetCursorPos(window, &inputInstance->lastX, &inputInstance->lastY);
            inputInstance->initialX = inputInstance->lastX;
            inputInstance->initialY = inputInstance->lastY;
        }
        else if (action == GLFW_RELEASE) {
            inputInstance->mousePressed = false;
        }
    }
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto* inputInstance = static_cast<Input*>(glfwGetWindowUserPointer(window));

    if (inputInstance->mousePressed) {
        double deltaX = xpos - inputInstance->lastX;
        double deltaY = ypos - inputInstance->lastY;

        // Adjust view matrix based on mouse movement
        const float sensitivity = 0.05f;
        inputInstance->viewMatrix = glm::translate(inputInstance->viewMatrix, glm::vec3(deltaX * sensitivity, -deltaY * sensitivity, 0.0f));

        inputInstance->lastX = xpos;
        inputInstance->lastY = ypos;
    }
}

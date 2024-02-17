#include "utils/window.h"
#include "utils/shader.h"
#include "utils/input.h"

int main() {
    Window window("Hello World", 680, 480);
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Input input(window, shader);

    while (!window.shouldClose()) {
        input.processInput();
        window.clear();

        shader.use();
        shader.setUniformMat4("view", input.getViewMatrix());

        input.drawGraph();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}



/*


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

double lastX = 0.0;
double lastY = 0.0;
bool mousePressed = false;
glm::mat4 viewMatrix = glm::mat4(1.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

int main(void)
{
    // Initialize the library
    if (!glfwInit())
        return -1;

    // Define GL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create a windowed mode window and its OpenGL context 
    GLFWwindow* window = glfwCreateWindow(680, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load OpenGL function pointers!" << std::endl;
        return -1;
        glfwTerminate();
    }


    // define vertex shader
    const char* vertexShaderSrc = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 view;\n"
        "void main() {\n"
            "gl_Position = view * vec4(aPos, 1.0);\n"
        "}\0";


    // define fragment shader
    const char* fragmentShaderSrc =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "     fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\0";


    // create vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, 0);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        std::cout << "Failed to compile vertex shader! ERR: " << infoLog << std::endl;
    }

    // create fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "Failed to compile fragment shader! ERR: " << infoLog << std::endl;
    }

    // create shader program and link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "Failed to link the shader program! ERR: " << infoLog << std::endl;
    }


    // delete shaders to free up resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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


    // create vertex buffer object with vertices data
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // look up GL_STREAM_DRAW and GL_DYNAMIC_DRAW
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //
    //   GL_STATIC_DRAW
    //       data set once and used many times
    // 
    //   GL_STREAM_DRAW 
    //       data set once and used by GPU at most a few times
    // 
    //   GL_DYNAMIC_DRAW
    //       data set many times and used many times

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        process_input(window);

        glClearColor(0.0f, 0.039f, 0.078f, 1.0f);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glBindVertexArray(VAO);
        

        // Draw the line graph
        glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);

        // Draw the x-axis
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        // Draw the y-axis
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(8 * sizeof(unsigned int)));


        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // free up memory in gpu and delete vbo/vao
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << "Window size is " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    // close window when escape is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastX, &lastY);
            std::cout << "Mouse Press" << std::endl;
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
            std::cout << "Mouse Release" << std::endl;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        double deltaX = xpos - lastX;
        double deltaY = ypos - lastY;

        // Adjust view matrix based on mouse movement
        // You may need to adjust the sensitivity of movement
        const float sensitivity = 0.05f;
        viewMatrix = glm::translate(viewMatrix, glm::vec3(deltaX * sensitivity, -deltaY * sensitivity, 0.0f));

        lastX = xpos;
        lastY = ypos;
    }
}

*/
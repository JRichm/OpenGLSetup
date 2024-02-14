#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load OpenGL function pointers!" << std::endl;
        return -1;
        glfwTerminate();
    }


    /* define vertex shader */
    const char* vertexShaderSrc = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\0";


    /* define fragment shader */
    const char* fragmentShaderSrc =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "     fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
        "}\0";


    /* create vertex shader */
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

    /* create fragment shader */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "Failed to compile fragment shader! ERR: " << infoLog << std::endl;
    }

    /* create shader program and link shaders */
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "Failed to link the shader program! ERR: " << infoLog << std::endl;
    }


    /* delete shaders to free up resources */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
         0.22f,  0.5f, 0.0f, // top right
         0.42f,  0.0f, 0.0f, // middle right
         0.22f, -0.5f, 0.0f, // bottom right
        -0.22f, -0.5f, 0.0f, // bottom left
        -0.42f,  0.0f, 0.0f, // middle left
        -0.22f,  0.5f, 0.0f  // top left
    };

    unsigned int indices[] = {
        0, 5, 4, // first triangle
        0, 4, 1, // second
        1, 4, 3, // third
        1, 3, 2  // fourth
    };


    /* create vertex buffer object with vertices data */
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); /* look up GL_STREAM_DRAW and GL_DYNAMIC_DRAW */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    /*
    *   GL_STATIC_DRAW
    *       data set once and used many times
    * 
    *   GL_STREAM_DRAW 
    *       data set once and used by GPU at most a few times
    * 
    *   GL_DYNAMIC_DRAW
    *       data set many times and used many times
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        process_input(window);

        glClearColor(0.914f, 0.722f, 0.141f, 1.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* free up memory in gpu and delete vbo/vao */
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
    /* close window when escape is pressed */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}
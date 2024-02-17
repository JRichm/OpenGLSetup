#include "shader.h"


// load shader, compile link, etc.
Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    unsigned int vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    ID = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// cleanup shaders, prgram, etc.
Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
	glUseProgram(ID);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


// read file and return contents
std::string Shader::readFile(const char* filePath) {

    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    content = sstr.str();

    fileStream.close();
    return content;
}


// compile shader and return shader ID
unsigned int Shader::compileShader(const char* source, unsigned int type) {

    unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed. Error: " << infoLog << std::endl;
    }

    return shaderID;
}


// link shaders and return program ID
unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {

    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed. Error: " << infoLog << std::endl;
    }

    return programID;
}
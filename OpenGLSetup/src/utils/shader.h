#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader();

		void use() const;
		void setUniformMat4(const std::string& name, const glm::mat4& value);
	
	private:
		unsigned int ID;

		std::string readFile(const char* filePath);
		unsigned int compileShader(const char* source, unsigned int type);
		unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};
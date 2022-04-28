#include "shader.hpp"

#include <glad/glad.h>

#include <fstream>
#include <streambuf>

Shader::Shader() {
	m_ID = glCreateProgram();
}
Shader::~Shader() {
	glDeleteProgram(m_ID);
}

void Shader::LoadFromFile(const std::string& _filename) {
	std::ifstream vfile(_filename + ".vs");
	std::string vertexShaderSource((std::istreambuf_iterator<char>(vfile)), std::istreambuf_iterator<char>());
	std::ifstream ffile(_filename + ".fs");
	std::string fragmentShaderSource((std::istreambuf_iterator<char>(ffile)), std::istreambuf_iterator<char>());
	
	char* vertexShaderSourcePointer = &vertexShaderSource[0];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourcePointer, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Vertex Shader Compilation error!\n" << infoLog << std::endl;
	}

	char* fragmentShaderSourcePointer = &fragmentShaderSource[0];
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourcePointer, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Fragment Shader Compilation error!\n" << infoLog << std::endl;
	}

	printf("< > VertexShaderSourceCode:\n%s\n< > FragmentShaderSourceCode:\n%s\n", vertexShaderSourcePointer, fragmentShaderSourcePointer);

	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cerr << "Shader Program Linking error!\n" << infoLog << std::endl;
	}
	glUseProgram(m_ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUniform1i(glGetUniformLocation(m_ID, "u_texid"), 0);
}

void Shader::Bind() const {
	glUseProgram(m_ID);
}

void Shader::Set4x4f(const std::string& _name, const Math::Mat4& _v) {
	glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, &_v[0]);
}
void Shader::Set4f(const std::string& _name, const Math::Vec4& _v) {
	glUniform4f(GetUniformLocation(_name), _v.x, _v.y, _v.z, _v.w);

}
void Shader::Set3f(const std::string& _name, const Math::Vec3& _v) {
	glUniform3f(GetUniformLocation(_name), _v.x, _v.y, _v.z);

}
void Shader::Set2f(const std::string& _name, const Math::Vec2& _v) {
	glUniform2f(GetUniformLocation(_name), _v.x, _v.y);

}
void Shader::Set1f(const std::string& _name, float _v) {
	glUniform1f(GetUniformLocation(_name), _v);
}

int Shader::GetUniformLocation(const std::string& _name) {
	// If key exists, return what it holds
	if(m_UniformLocations.find(_name) != m_UniformLocations.end()) {
		return m_UniformLocations[_name];
	}
	// If the key does not yet exist, generate it
	int uniformLocation = glGetUniformLocation(m_ID, _name.c_str());
	m_UniformLocations[_name] = uniformLocation;
	return uniformLocation;
}
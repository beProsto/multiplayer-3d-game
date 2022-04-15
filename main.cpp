#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>

#include "math/math.hpp"

int Main(const std::vector<std::string>& _args) {
	OWL::GLContext context;
	OWL::Window window(&context, "OWL + GLAD = <3", OWL::Vec2i(0), OWL::Vec2i(1280, 720));
	window.SetMaxGamepads(0);

	if(!gladLoadGLLoader((GLADloadproc)OWL::GLContext::GetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		
		 0.0f, -0.5f, -0.5f,
		 0.0f,  0.5f, -0.5f,
		 0.0f,  0.5f,  0.5f,
		 
		 0.0f,  0.5f,  0.5f,
		 0.0f, -0.5f,  0.5f,
		 0.0f, -0.5f, -0.5f,
	};

	std::string vertexShaderSource = R"V0G0N(#version 330 core
		layout(location = 0) in vec3 a_pos;
		
		uniform mat4 u_mat;

		out vec3 v_col;

		void main() {
			gl_Position = u_mat * vec4(a_pos, 1.0);
			v_col = vec3(a_pos+0.5);
		}
	)V0G0N";
	std::string fragmentShaderSource = R"V0G0N(#version 330 core
		layout(location = 0) out vec4 col;

		in vec3 v_col;

		void main() {
			col = vec4(v_col, 1.0);
		}
	)V0G0N";
	

	std::cout << "SHADER SOURCE CODE:\n" << vertexShaderSource << std::endl << fragmentShaderSource << std::endl;

	unsigned int vertexArrayObject = 0;
	typedef void(*ptr)(unsigned int, unsigned int*);
	((ptr)OWL::GLContext::GetProcAddress("glGenVertexArrays"))(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	unsigned int vertexBufferObject = 0;
	((ptr)OWL::GLContext::GetProcAddress("glGenBuffers"))(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// first attribute is at location=0, it's made of 2 floats, the stride has byte size of 2*4(size of 32 bit float) and the attribute is at offset 0 from the stride's start
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	char* vertexShaderSourcePointer = &vertexShaderSource[0];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourcePointer, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Compilation error!\n" << infoLog << std::endl;
		return 1;
	}

	char* fragmentShaderSourcePointer = &fragmentShaderSource[0];
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourcePointer, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compilation error!\n" << infoLog << std::endl;
		return 2;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Program Linking error!\n" << infoLog << std::endl;
		return 3;
	}
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_DEPTH_TEST);
	
	Math::Mat4 transform;
	Math::Mat4 projection;

	OWL::Vec2f lastPosition = window.Mouse.GetPosition();
	OWL::Vec2f rotation;

	float scale = -2.0f;

	OWL::FPSLimiter eventDelay(60);
	while(window.IsRunning()) {
		eventDelay.Start();
		window.PollEvents();

		if(window.Keyboard.IsKeyPressed(window.Keyboard.Escape)) {
			window.Close();
		}
		if(window.Keyboard.GetKeyData().KeyEnum == window.Keyboard.F11) {
			window.SetFullScreen(!window.IsFullScreen());
		}

		scale += window.Mouse.GetWheelRotation() / 1000.0f;

		glViewport(0, 0, window.GetSize().x, window.GetSize().y);
		glClearColor(0.2f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Math::_Mat4::RotateZ(&rotation[0], 0.1f);
		projection = Math::Mat4::Perspective(M_PI/2.0f, window.GetAspect(), 0.1f, 100.0f) * Math::Mat4::Translate(0.0f, 0.0f, scale);

		OWL::Vec2f thisPosition = window.Mouse.GetPosition();
		OWL::Vec2f diff = thisPosition - lastPosition;
		lastPosition = thisPosition;

		if(window.Mouse.IsButtonPressed(OWL::Window::MouseEvent::Left)) {
			rotation += OWL::Vec2f(diff.y, diff.x) * OWL::Vec2f(0.01f);
			transform = Math::Mat4::RotateX(rotation.x) * Math::Mat4::RotateY(rotation.y);
		}

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_mat"), 1, GL_FALSE, &(projection * transform)[0]);

		glDrawArrays(GL_TRIANGLES, 0, 12);

		context.SwapBuffers(); 
		eventDelay.End();
	}

	return 0;
}

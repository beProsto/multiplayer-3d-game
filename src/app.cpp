#include "app.hpp"

App::App(OWL::Window& _window, OWL::GLContext& _context):
m_Context(_context),
m_Window(_window) {
}
App::~App() {
}


void App::Start() {
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);

	m_Mesh.SupplyIndices({0, 1, 2, 2, 3, 0});
	m_Mesh.SupplyArray(0, 3, {
		-0.5, -0.5, 0.0,
		-0.5,  0.5, 0.0,
		+0.5,  0.5, 0.0,
		+0.5, -0.5, 0.0,
	});
	m_Mesh.SupplyArray(1, 3, {
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	});

	std::string vertexShaderSource = R"V0G0N(#version 330 core
		layout(location = 0) in vec3 a_pos;
		layout(location = 1) in vec3 a_col;
		
		out vec3 v_col;

		uniform mat4 u_mat;

		void main() {
			gl_Position = u_mat * vec4(a_pos, 1.0);
			v_col = vec3(a_col);
		}
	)V0G0N";
	std::string fragmentShaderSource = R"V0G0N(#version 330 core
		in vec3 v_col;

		layout(location = 0) out vec4 col;

		void main() {
			col = vec4(v_col, 1.0);
		}
	)V0G0N";

	std::cout << "SHADER SOURCE CODE:\n" << vertexShaderSource << std::endl << fragmentShaderSource << std::endl;

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
		m_Window.Close();
	}

	char* fragmentShaderSourcePointer = &fragmentShaderSource[0];
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourcePointer, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compilation error!\n" << infoLog << std::endl;
		m_Window.Close();
	}

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Program Linking error!\n" << infoLog << std::endl;
		m_Window.Close();
	}
	glUseProgram(m_ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	m_LastPosition = m_Window.Mouse.GetPosition();

	m_Scale = -2.0f;
}

void App::Update() {
	// Debugging
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::Escape) {
		m_Window.Close();
	}
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::F11) {
		m_Window.SetFullScreen(!m_Window.IsFullScreen());
	}

	// Makeshift rendering for now;
	glViewport(0, 0, m_Window.GetSize().x, m_Window.GetSize().y);
	glClear(GL_COLOR_BUFFER_BIT);

	m_Scale += m_Window.Mouse.GetWheelRotation() / 1000.0f;
	m_Projection = Math::Mat4::Perspective(M_PI/2.0f, m_Window.GetAspect(), 0.1f, 100.0f) * Math::Mat4::Translate(0.0f, 0.0f, m_Scale);
	OWL::Vec2f thisPosition = m_Window.Mouse.GetPosition();
	OWL::Vec2f diff = thisPosition - m_LastPosition;
	m_LastPosition = thisPosition;
	if(m_Window.Mouse.IsButtonPressed(OWL::Window::MouseEvent::Left)) {
		m_Rotation += OWL::Vec2f(diff.y, diff.x) * OWL::Vec2f(0.01f);
		m_Transform = Math::Mat4::RotateX(m_Rotation.x) * Math::Mat4::RotateY(m_Rotation.y);
	}
	Math::Mat4 identity = m_Projection * m_Transform;
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_mat"), 1, GL_FALSE, &(identity)[0]);

	m_Mesh.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	m_Context.SwapBuffers();
}

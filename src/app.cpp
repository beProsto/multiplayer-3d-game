#include "app.hpp"

App::App(OWL::Window& _window, OWL::GLContext& _context):
m_Context(_context),
m_Window(_window) {
}
App::~App() {
}


void App::Start() {
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
	m_Mesh.SupplyIndices(std::vector<unsigned int>{0, 1, 2});
	m_Mesh.SupplyArray(0, 3, std::vector<float>{
		-0.5, -0.5, 0.0,
		 0.0,  0.5, 0.0,
		 0.5, -0.5, 0.0
	});
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

	glDrawElements(GL_TRIANGLES, m_Mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);

	m_Context.SwapBuffers();
}

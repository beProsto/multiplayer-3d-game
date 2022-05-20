#include "renderer.hpp"

#include <glad/glad.h>

Renderer::Renderer() {
	for(size_t i = 0; i < ShaderType::count; i++) {
		m_Shaders[i].LoadFromFile("assets/shaders/" + ShaderTypeToString((ShaderType)i));
	}
	
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

Renderer::~Renderer() {

}

void Renderer::SetSceneLight(Light& _light) {
	m_Light = &_light;
}
void Renderer::SetSceneCamera(Camera& _camera) {
	m_Camera = &_camera;
}

void Renderer::BeginScene(uint32_t _width, uint32_t _height, float _aspect) {
	m_Camera->SetAspectRatio(_aspect);
	glViewport(0,0,_width,_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(size_t i = 0; i < ShaderType::count; i++) {
		m_Shaders[i].Bind();
		m_Shaders[i].Set3f("u_lightcolor", m_Light->color);
		m_Shaders[i].Set1f("u_lightambient", m_Light->ambient);
		m_Shaders[i].Set3f("u_lightdir", m_Light->direction);
		m_Shaders[i].Set4x4f("u_cam", m_Camera->GetMatrix());
	}
}

void Renderer::Draw(uint32_t _shader, const Mesh& _mesh, const Texture& _texture, const Math::Mat4& _transform) {
	m_Shaders[_shader].Bind();
	m_Shaders[_shader].Set4x4f("u_mat", _transform);
	_texture.Bind();
	_mesh.Draw();
}
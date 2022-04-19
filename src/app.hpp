#pragma once

#include <glad/glad.h>
#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>
#include "math/math.hpp"

#include "rendering/mesh.hpp"

class App {
public:
	App(OWL::Window& _window, OWL::GLContext& _context);
	~App();

	void Start();
	void Update();

protected:
	OWL::GLContext& m_Context;
	OWL::Window& m_Window;

	unsigned int shaderProgram;
	float scale;
	Math::Mat4 transform;
	Math::Mat4 projection;
	OWL::Vec2f lastPosition;
	OWL::Vec2f rotation;

	Mesh m_Mesh;
};
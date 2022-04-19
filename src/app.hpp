#pragma once

#include <glad/glad.h>
#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>
#include "math/math.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include "rendering/mesh.hpp"

struct Vertex {
	float x, y, z;
	float i, j, k;
};

class App {
public:
	App(OWL::Window& _window, OWL::GLContext& _context);
	~App();

	void Start();
	void Update();

protected:
	OWL::GLContext& m_Context;
	OWL::Window& m_Window;

	// std::vector<float> m_Vertices;
	// std::vector<unsigned int> m_Indices;
	// unsigned int m_IndexBuffer;
	// unsigned int m_ArrayBuffers[2];
	// unsigned int m_VertexArray;
	unsigned int m_ShaderProgram;
	
	float m_Scale;
	Math::Mat4 m_Transform;
	Math::Mat4 m_Projection;
	OWL::Vec2f m_LastPosition;
	OWL::Vec2f m_Rotation;

	ALCdevice *m_ALCDev;
	ALCcontext *m_ALCCtx;
	ALuint m_ALBuffers[1];
	ALuint m_ALSources[1];


	Mesh m_Mesh;
};
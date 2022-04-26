#pragma once

#include <glad/glad.h>

#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include "math/math.hpp"
#include "rendering/mesh.hpp"
#include "rendering/texture.hpp"

#include "camera.hpp"

#include "transform.hpp"

#include "audio/audio.hpp"

struct Vertex {
	float x, y, z;
	float i, j, k;
};

class App {
public:
	App(OWL::Window& _window, OWL::GLContext& _context);
	~App();

	void Start();
	void Update(float _dt);

protected:
	OWL::GLContext& m_Context;
	OWL::Window& m_Window;

	unsigned int m_ShaderProgram;

	Camera m_Camera;

	AudioManager m_Audio;
	ALuint m_ALBuffers[1];
	ALuint m_ALSources[1];

	bool m_IsMouseLocked;
	Transform m_LightTranform;

	Texture m_BaseTexture;
	Texture m_MapTexture;
	Texture m_MeshTexture;

	Mesh m_Plane;
	Mesh m_Map;
	Mesh m_Mesh;
};
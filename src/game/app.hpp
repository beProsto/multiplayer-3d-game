#pragma once

#include <thread>

#include <glad/glad.h>

#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>

#define SUS_DEBUG_OFF
#include "../common/sus/networking.hpp"

#include "../common/math/math.hpp"
#include "rendering/renderer.hpp"

#include "transform.hpp"

#include "audio/audio.hpp"

struct Vertex {
	float x, y, z;
	float i, j, k;
};

class App {
public:
	App(OWL::Window& _window, OWL::GLContext& _context, SUS::Client& _network);
	~App();

	void Start();
	void Update(float _dt);

protected:
	OWL::GLContext& m_Context;
	OWL::Window& m_Window;
	SUS::Client m_Network;

	bool m_NetworkThreadRunning;
	std::thread m_NetworkThread;
	
	std::unordered_map<SOCKET, Vertex> m_PlayersPositions;

	Camera m_Camera;
	Light m_SunLight;
	Renderer m_Renderer;

	AudioManager m_Audio;
	AudioSource m_Sound;

	bool m_IsMouseLocked;
	Transform m_LightTranform;

	Texture m_BaseTexture;
	Texture m_MapTexture;
	Texture m_MeshTexture;

	Mesh m_Plane;
	Mesh m_Map;
	Mesh m_Mesh;
	Mesh m_Robot;
	
	Math::Mat4 m_Identity;
};
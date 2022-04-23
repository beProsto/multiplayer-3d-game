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

struct Vertex {
	float x, y, z;
	float i, j, k;
};

struct WAVHeader {
	uint32_t riff;
	uint32_t size, chunkSize;
	int16_t formatType, channels;
	uint32_t sampleRate, avgBytesPerSec;
	int16_t bytesPerSample, bitsPerSample;
	uint32_t dataSize;
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

	// std::vector<float> m_Vertices;
	// std::vector<unsigned int> m_Indices;
	// unsigned int m_IndexBuffer;
	// unsigned int m_ArrayBuffers[2];
	// unsigned int m_VertexArray;
	unsigned int m_ShaderProgram;
	
	// float m_Scale;
	// Math::Mat4 m_Transform;
	// Math::Mat4 m_Projection;
	// OWL::Vec2f m_LastPosition;
	// OWL::Vec2f m_Rotation;

	Camera m_Camera;

	WAVHeader m_AudioFileHeader;
	std::vector<unsigned char> m_AudioFileData;

	ALCdevice* m_ALCDev;
	ALCcontext* m_ALCCtx;
	ALuint m_ALBuffers[1];
	ALuint m_ALSources[1];

	bool m_IsMouseLocked;

	Texture m_BaseTexture;
	Texture m_MeshTexture;

	Mesh m_Plane;
	Mesh m_Mesh;
};
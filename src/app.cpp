#include "app.hpp"

#include "rendering/loaderOBJ.hpp"

App::App(OWL::Window& _window, OWL::GLContext& _context):
m_Context(_context),
m_Window(_window) {
}
App::~App() {
}

void App::Start() {
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);

	m_Plane.SupplyIndices({0, 1, 2, 2, 3, 0});
	m_Plane.SupplyArray(0, 3, {
		-1.0f, -2.0f, -1.0f,
		-1.0f, -2.0f, +1.0f,
		+1.0f, -2.0f, +1.0f,
		+1.0f, -2.0f, -1.0f
	});
	m_Plane.SupplyArray(1, 2, {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	});
	m_Plane.SupplyArray(2, 3, {
		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, +1.0f,
		+1.0f, 0.0f, +1.0f,
		+1.0f, 0.0f, -1.0f
	});

	LoaderOBJ::Model model = LoaderOBJ::Load("assets/beryla.obj");
	m_Mesh.SupplyIndices(model.indices);
	m_Mesh.SupplyArray(0, 3, model.positions);
	m_Mesh.SupplyArray(1, 2, model.texcoords);
	m_Mesh.SupplyArray(2, 3, model.normals);

	unsigned char white[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	m_BaseTexture.LoadFromData(white, 1, 1);

	m_MeshTexture.LoadFromFile("assets/beryl.png");


	std::string vertexShaderSource = R"V0G0N(#version 330 core
		layout(location = 0) in vec3 a_pos;
		layout(location = 1) in vec2 a_tex;
		layout(location = 2) in vec3 a_nor;
		
		out vec2 v_tex;

		uniform mat4 u_mat;

		void main() {
			gl_Position = u_mat * vec4(a_pos, 1.0);
			v_tex = a_tex;
			v_tex.y = 1.0 - v_tex.y;
		}
	)V0G0N";
	std::string fragmentShaderSource = R"V0G0N(#version 330 core
		in vec2 v_tex;

		layout(location = 0) out vec4 col;

		uniform sampler2D u_texid;

		void main() {
			col = texture(u_texid, v_tex);
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

	glUniform1i(glGetUniformLocation(m_ShaderProgram, "u_texid"), 0);

	glEnable(GL_DEPTH_TEST);

	m_Camera.SetTranslation(Math::Vec3(0.0f, 0.0f, 1.0f));

	m_Window.Mouse.SetVisibility(false);

	m_IsMouseLocked = true;

	/***
	 * 	WAVE AUDIO FILE LOADING BEGIN
	***/
	FILE* audioFile = fopen("assets/audio.wav", "rb");

	if(audioFile == nullptr) {
		fprintf(stderr, "The file couldn't be loaded!\n");
		m_Window.Close();
		return;
	}
	
	fread(&m_AudioFileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(m_AudioFileHeader.riff != 1179011410 /* Non null terminated "RIFF" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'RIFF' found)\n");
		m_Window.Close();
		return;
	}

	fread(&m_AudioFileHeader.size, sizeof(uint32_t), 1, audioFile);

	fread(&m_AudioFileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(m_AudioFileHeader.riff != 1163280727 /* Non null terminated "WAVE" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'WAVE' found)\n");
		m_Window.Close();
		return;
	}

	fread(&m_AudioFileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(m_AudioFileHeader.riff != 544501094 /* Non null terminated "fmt " string */) {
		fprintf(stderr, "The file isn't WAV! (No 'fmt ' found)\n");
		m_Window.Close();
		return;
	}

	fread(&m_AudioFileHeader.chunkSize, sizeof(uint32_t), 1, audioFile);
	fread(&m_AudioFileHeader.formatType, sizeof(int16_t), 1, audioFile);
	fread(&m_AudioFileHeader.channels, sizeof(int16_t), 1, audioFile);
	fread(&m_AudioFileHeader.sampleRate, sizeof(uint32_t), 1, audioFile);
	fread(&m_AudioFileHeader.avgBytesPerSec, sizeof(uint32_t), 1, audioFile);
	fread(&m_AudioFileHeader.bytesPerSample, sizeof(int16_t), 1, audioFile);
	fread(&m_AudioFileHeader.bitsPerSample, sizeof(int16_t), 1, audioFile);

	fread(&m_AudioFileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(m_AudioFileHeader.riff == 1414744396 /* Non null terminated "LIST" string */) {
		fprintf(stderr, "Parsing through LIST...\n");

		uint32_t sizeToSkip = 0;
		fread(&sizeToSkip, sizeof(uint32_t), 1, audioFile);
		fprintf(stderr, "Skipping %u bytes...\n", sizeToSkip);
		fseek(audioFile, sizeToSkip, SEEK_CUR);

		fread(&m_AudioFileHeader.riff, sizeof(uint32_t), 1, audioFile);
	}
	if(m_AudioFileHeader.riff != 1635017060 /* Non null terminated "data" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'data' found)\n");
		m_Window.Close();
		return;
	}

	fread(&m_AudioFileHeader.dataSize, sizeof(uint32_t), 1, audioFile);
	fprintf(stderr, "Reading %u bytes of WAVE data...\n", m_AudioFileHeader.dataSize);

	// Here's where the data parsing begins
	m_AudioFileData.resize(m_AudioFileHeader.dataSize);
	fread(m_AudioFileData.data(), sizeof(unsigned char), m_AudioFileHeader.dataSize, audioFile);
	/***
	 * 	WAVE AUDIO FILE LOADING END
	***/

	m_ALCDev = alcOpenDevice(nullptr);
	if(m_ALCDev == nullptr) {
		fprintf(stderr, "Failed to initialise OpenAL!\n");
		m_Window.Close();
		return;
	}

	m_ALCCtx = alcCreateContext(m_ALCDev, nullptr);
	alcMakeContextCurrent(m_ALCCtx);
	if(m_ALCDev == nullptr) {
		fprintf(stderr, "Failed to create an OpenAL context!\n");
		m_Window.Close();
		return;
	}

	ALuint frequency = m_AudioFileHeader.sampleRate;
	ALenum format    = 0;

	alGenBuffers(1, m_ALBuffers);
	alGenSources(1, m_ALSources);

	// you may ask why
	// for i am lazy
	// thus i tasked myself with finding an easier way
	// and came up with something that took even longer
	format = 0x1100;
	format += ((m_AudioFileHeader.channels - 1 ) * 2) + ((m_AudioFileHeader.bitsPerSample / 8) - 1);

	if(format == AL_FORMAT_MONO8) {
		std::cout << "MONO8\n";
	}
	else if(format == AL_FORMAT_MONO16) {
		std::cout << "MONO16\n";
	}
	else if(format == AL_FORMAT_STEREO8) {
		std::cout << "STEREO8\n";
	}
	else if(format == AL_FORMAT_STEREO16) {
		std::cout << "STEREO16\n";
	}
	else {
		std::cout << "Unknown Format!\n";
	}

	alBufferData(m_ALBuffers[0], format, m_AudioFileData.data(), m_AudioFileHeader.dataSize, frequency);

	alSourcei(m_ALSources[0], AL_BUFFER, m_ALBuffers[0]);
	alSourcei(m_ALSources[0], AL_LOOPING, true);
	alSourcePlay(m_ALSources[0]);
}

void App::Update(float _dt) {
	// Debugging
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::Escape) {
		m_Window.Mouse.SetVisibility(!m_Window.Mouse.IsVisible());
		m_IsMouseLocked = !m_IsMouseLocked;
	}
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::F11) {
		m_Window.SetFullScreen(!m_Window.IsFullScreen());
	}

	// Makeshift rendering for now;
	glViewport(0, 0, m_Window.GetSize().x, m_Window.GetSize().y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Camera.SetAspectRatio(m_Window.GetAspect());

	if(m_IsMouseLocked) {
		OWL::Vec2ui windowCenter = m_Window.GetSize() / OWL::Vec2ui(2);
		OWL::Vec2f mouse = m_Window.Mouse.GetPosition() - windowCenter;
		mouse *= OWL::Vec2f(0.01f);
		m_Window.Mouse.SetPosition(windowCenter);

		Math::Vec3 rotation = m_Camera.GetRotation() - Math::Vec3(mouse.y, mouse.x, 0.0f) * 0.5f;
		rotation.x = max(min(rotation.x, M_PI/2.0), -M_PI/2.0);
		m_Camera.SetRotation(rotation);
	}

	float movementX = (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::D) - (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::A);
	float movementZ = (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::S) - (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::W);

	float velX = cos(m_Camera.GetRotation().y) * movementX + sin(m_Camera.GetRotation().y) * movementZ;
	float velZ = sin(m_Camera.GetRotation().y) * -movementX + cos(m_Camera.GetRotation().y) * movementZ;

	m_Camera.SetTranslation(m_Camera.GetTranslation() + Math::Vec3(velX, 0.0f, velZ) * _dt * 10.0f);

	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_mat"), 1, GL_FALSE, &(m_Camera.GetMatrix())[0]);

	Math::Vec3 translation = m_Camera.GetTranslation();

	Math::Vec4 stand = m_Camera.GetViewMatrix() * Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// stand /= stand.w;
	Math::Vec4 up = m_Camera.GetViewMatrix() * Math::Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	// up /= up.w;
	Math::Vec4 front = m_Camera.GetViewMatrix() * Math::Vec4(0.0f, 0.0f, -1.0f, 1.0f);
	// front /= front.w;

	up = up - stand;
	front = front - stand;

	// float faceX = sin(m_Camera.GetRotation().y) * -1.0f;
	// float faceZ = cos(m_Camera.GetRotation().y) * -1.0f;

	// Math::Vec3 upNorm = Math::Vec3::Normalise(Math::Vec3(0.0f,1.0f,0.0f));
	// Math::Vec3 frNorm = Math::Vec3::Normalise(Math::Vec3(faceX,0.0f,faceZ));

	Math::Vec3 upNorm = Math::Vec3::Normalise(Math::Vec3(up.x,up.y,up.z));
	Math::Vec3 frNorm = Math::Vec3::Normalise(Math::Vec3(front.x,front.y,front.z));

	printf("<>FRONT:%f,%f,%f\n", frNorm.x, frNorm.y, frNorm.z);
	printf("<>UPWRD:%f,%f,%f\n", upNorm.x, upNorm.y, upNorm.z);

	float ori[6] = {
		frNorm.x, frNorm.y, frNorm.z,
		upNorm.x, upNorm.y, upNorm.z
	};
	
	alListenerfv(AL_ORIENTATION, ori);
	alListener3f(AL_POSITION, translation.x, translation.y, translation.z);
	
	m_MeshTexture.Bind(0);
	m_Mesh.Bind();
	glDrawElements(GL_TRIANGLES, m_Mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);

	m_BaseTexture.Bind(0);
	m_Plane.Bind();
	glDrawElements(GL_TRIANGLES, m_Plane.GetIndexCount(), GL_UNSIGNED_INT, 0);

	m_Context.SwapBuffers();
}

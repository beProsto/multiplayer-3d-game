#include "app.hpp"

#include "audio/loaderWAV.hpp"

App::App(OWL::Window& _window, OWL::GLContext& _context, SUS::Client& _network):
m_Context(_context),
m_Window(_window),
m_Network(_network) {
}
App::~App() {
}

// void d_printm4(Math::Mat4 m) {
// 	printf("< MATRIX X x X >\n");
// 	printf("%f, %f, %f, %f\n", m[0], m[1], m[2], m[3]);
// 	printf("%f, %f, %f, %f\n", m[4], m[5], m[6], m[7]);
// 	printf("%f, %f, %f, %f\n", m[8], m[9], m[10], m[11]);
// 	printf("%f, %f, %f, %f\n", m[12], m[13], m[14], m[15]);
// 	printf("---------------\n");
// }

void App::Start() {
	m_Plane.SupplyIndices({0, 1, 2, 2, 3, 0});
	m_Plane.SupplyArray(0, 3, {
		-1.0f, -1.9f, -1.0f,
		-1.0f, -1.9f, +1.0f,
		+1.0f, -1.9f, +1.0f,
		+1.0f, -1.9f, -1.0f
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

	LoaderOBJ::Model model = LoaderOBJ::Load("assets/beryl.obj");
	m_Mesh.SupplyIndices(model.indices);
	m_Mesh.SupplyArray(0, 3, model.positions);
	m_Mesh.SupplyArray(1, 2, model.texcoords);
	m_Mesh.SupplyArray(2, 3, model.normals);

	model = LoaderOBJ::Load("assets/map_mntns.obj");
	m_Map.SupplyIndices(model.indices);
	m_Map.SupplyArray(0, 3, model.positions);
	m_Map.SupplyArray(1, 2, model.texcoords);
	m_Map.SupplyArray(2, 3, model.normals);

	unsigned char white[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	m_BaseTexture.LoadFromData(white, 1, 1);
	m_MeshTexture.LoadFromFile("assets/beryl.png");
	m_MapTexture.LoadFromFile("assets/map_mntns.png");

	m_Camera.SetTranslation(Math::Vec3(0.0f, 0.0f, 1.0f));

	m_Window.Mouse.SetVisibility(false);
	m_IsMouseLocked = true;

	m_LightTranform = Transform(Math::Vec3(), Math::Vec3(), Math::Vec3());
	m_SunLight = Light{Math::Vec3(1.0f, 1.0f, 1.0f), 0.1f, Math::Vec3(m_LightTranform.Translation.x, -1.0f, m_LightTranform.Translation.y)};

	m_Renderer.SetSceneLight(m_SunLight);
	m_Renderer.SetSceneCamera(m_Camera);

	/***
	 * 	WAVE AUDIO FILE LOADING
	***/
	AudioData audio = LoaderWAV::Load("assets/audio.wav");
	m_Sound.SetData(audio);
	m_Sound.SetLooped(true);
	m_Sound.Play();

	m_NetworkThreadRunning = true;
	m_NetworkThread = std::thread([&]() {
		// Networking
		while(!m_Network.IsConnected() && m_NetworkThreadRunning) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(500ms);
			m_Network.Connect();
			std::cout << "Not yet hast we connected" << "\n";
		}
		while(m_NetworkThreadRunning) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(20ms);
			m_Network.Update();
			SUS::Event event;
			while(m_Network.PollEvent(event)) {
				std::cout << "AJJJJJJ MSESASGE" << "\n";
				m_Network.Send(32);
			}
		}
	});
}

void App::Update(float _dt) {
	// Debugging
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::Escape) {
		m_Window.Close();
		m_NetworkThreadRunning = false;
		m_NetworkThread.join();
	}
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::BackQuote) {
		m_Window.Mouse.SetVisibility(!m_Window.Mouse.IsVisible());
		m_IsMouseLocked = !m_IsMouseLocked;
	}
	if(m_Window.Keyboard.GetKeyData().KeyEnum == OWL::Window::KeyboardEvent::F11) {
		m_Window.SetFullScreen(!m_Window.IsFullScreen());
	}

	if(m_IsMouseLocked) {
		OWL::Vec2ui windowCenter = m_Window.GetSize() / OWL::Vec2ui(2);
		OWL::Vec2f mouse = m_Window.Mouse.GetPosition() - windowCenter;
		mouse *= OWL::Vec2f(0.01f);
		m_Window.Mouse.SetPosition(windowCenter);

		Math::Vec3 rotation = m_Camera.GetRotation() - Math::Vec3(mouse.y, mouse.x, 0.0f) * 0.3f;
		rotation.x = max(min(rotation.x, M_PI/2.0), -M_PI/2.0);
		m_Camera.SetRotation(rotation);
	}

	float movementX = (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::D) - (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::A);
	float movementZ = (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::S) - (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::W);
	float movementY = (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::Space) - (float)m_Window.Keyboard.IsKeyPressed(OWL::Window::KeyboardEvent::ShiftLeft);
	float velX = cos(m_Camera.GetRotation().y) * movementX + sin(m_Camera.GetRotation().y) * movementZ;
	float velZ = sin(m_Camera.GetRotation().y) * -movementX + cos(m_Camera.GetRotation().y) * movementZ;
	m_Camera.SetTranslation(m_Camera.GetTranslation() + Math::Vec3(velX, movementY, velZ) * _dt * 30.0f);

	m_Audio.SetListener(m_Camera.GetTranslation(), m_Camera.GetRotation());

	m_LightTranform.Translation.z += _dt/5.0f;
	m_LightTranform.Translation.x = sin(m_LightTranform.Translation.z)*2.0f-1.0f;
	m_LightTranform.Translation.y = cos(m_LightTranform.Translation.z)*2.0f-1.0f;
	m_SunLight.direction = Math::Vec3(m_LightTranform.Translation.x, -1.0f, m_LightTranform.Translation.y);


	// Makeshift rendering for now;
	m_Renderer.BeginScene(m_Window.GetSize().x, m_Window.GetSize().y, m_Window.GetAspect());

	m_Renderer.Draw(Renderer::ShaderType::basic, m_Mesh, m_MeshTexture, Transform(m_Camera.GetTranslation(), m_Camera.GetRotation()).GetMatrix() * Transform(Math::Vec3(0.05f, -0.06f, -0.1f), Math::Vec3(), Math::Vec3(0.04f)).GetMatrix());

	m_Renderer.Draw(Renderer::ShaderType::basic, m_Map, m_MapTexture, Math::Mat4::Translate(0.0f, -2.5f, 0.0f) * Math::Mat4::Scale(5.0f));

	m_Renderer.Draw(Renderer::ShaderType::inv, m_Plane, m_BaseTexture, m_Identity);

	m_Context.SwapBuffers();
}

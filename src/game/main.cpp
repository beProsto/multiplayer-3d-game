#include "app.hpp"
#include <chrono>

#define DEBUG

int Main(const std::vector<std::string>& _args) {
	#if defined(WIN32) && defined(DEBUG)
	if(AllocConsole()) {
		FILE* fi = 0;
		freopen_s(&fi, "CONOUT$", "w", stdout);
	}
	#endif

	std::string networkHostAddress = "127.0.0.1";

	if(_args.size() >= 2) {
		networkHostAddress = _args[1];
	}

	SUS::Client network(networkHostAddress);

	OWL::GLContext context;
	OWL::Window window(&context, "Multiplayer 3D Gaem ^^", OWL::Vec2(0), OWL::Vec2ui(1280, 720));
	window.SetMaxGamepads(0);
	
	// Initialise GLAD	
	if(!gladLoadGLLoader((GLADloadproc)OWL::GLContext::GetProcAddress)) {
		std::cerr << "GLAD: Couldn't initialise!" << std::endl;
		return -1;
	}

	App app(window, context, network);
	app.Start();

	auto timeLast = std::chrono::high_resolution_clock::now();
	while(window.IsRunning()) {
		auto timeNow = std::chrono::high_resolution_clock::now();
		double ms = std::chrono::duration<double, std::milli>(timeNow-timeLast).count();
		timeLast = timeNow;
		
		window.PollEvents();
		app.Update(ms/1000.0f);
	}
	// system("PAUSE");
	return 0;
}
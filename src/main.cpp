#include "app.hpp"
#include <chrono>

#include "networking/networking.hpp"

#define DEBUG

int Main(const std::vector<std::string>& _args) {
	#if defined(WIN32) && defined(DEBUG)
	if(AllocConsole()) {
		FILE* fi = 0;
		freopen_s(&fi, "CONOUT$", "w", stdout);
	}
	#endif

	if(_args.size() >= 2) {
		if(_args[1] == "client") {
			NetworkClient net;
		}
		else if(_args[1] == "server") {
			NetworkServer net;
		}
		system("PAUSE");
		return 1;
	}

	OWL::GLContext context;
	OWL::Window window(&context, "Multiplayer 3D Gaem ^^");
	window.SetMaxGamepads(0);
	
	// Initialise GLAD	
	if(!gladLoadGLLoader((GLADloadproc)OWL::GLContext::GetProcAddress)) {
		std::cerr << "GLAD: Couldn't initialise!" << std::endl;
		return -1;
	}

	App app(window, context);
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
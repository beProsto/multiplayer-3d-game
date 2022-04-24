#include "app.hpp"
#include <chrono>


int Main(const std::vector<std::string>& _args) {
	#ifdef WIN32
	if(AllocConsole()) {
		FILE* fi = 0;
		freopen_s(&fi, "CONOUT$", "w", stdout);
	}
	#endif
	
	OWL::GLContext context;
	OWL::Window window(&context, "Ekran Tytulowy ;3");
	window.SetMaxGamepads(0);
	
	// Initialise GLAD	
	if(!gladLoadGLLoader((GLADloadproc)OWL::GLContext::GetProcAddress)) {
		std::cerr << "GLAD: Couldn't initialise!" << std::endl;
		return -1;
	}
	
	auto tLast = std::chrono::high_resolution_clock::now();

	App app(window, context);
	// OWL::FPSLimiter limit(60);
	app.Start();
	while(window.IsRunning()) {
		auto tNow = std::chrono::high_resolution_clock::now();
		double ms = std::chrono::duration<double, std::milli>(tNow-tLast).count();
		tLast = tNow;
		// limit.Start();
		window.PollEvents();
		app.Update(ms/1000.0f);

		// std::cout << (float)(ms/1000.0f) << std::endl;
		// limit.End();
	}
	// system("PAUSE");
	return 0;
}
#include "app.hpp"

int Main(const std::vector<std::string>& _args) {
	OWL::GLContext context;
	OWL::Window window(&context);
	window.SetMaxGamepads(0);
	
	// Initialise GLAD	
	if(!gladLoadGLLoader((GLADloadproc)OWL::GLContext::GetProcAddress)) {
		std::cerr << "GLAD: Couldn't initialise!" << std::endl;
		return -1;
	}

	App app(window, context);
	// OWL::FPSLimiter limit(60);
	app.Start();
	while(window.IsRunning()) {
		// limit.Start();
		window.PollEvents();
		app.Update();
		// limit.End();
	}
	return 0;
}
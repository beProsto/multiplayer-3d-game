#ifndef _APP_HPP
#define _APP_HPP 

#include <glad/glad.h>
#include <OWL/OWL.hpp>
#include <OWL/time.hpp>
#include <OWL/GLContext.hpp>
#include "math/math.hpp"

struct AppData;

class App {
public:
	App();
	~App();

	float getA();

private:
	AppData* m_Data;
};

#endif
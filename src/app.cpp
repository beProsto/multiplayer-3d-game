#include "app.hpp"

struct AppData {
	float x;
};

App::App() {
	m_Data = new AppData;
	m_Data->x = 2.0f;
}
App::~App() {
	delete m_Data;
}
float App::getA() {
	return m_Data->x;
}
#pragma once

#include <string>

class Texture {
public:
	Texture();
	~Texture();

	void LoadFromData(unsigned char* _data, unsigned int _width, unsigned int _height);
	void LoadFromFile(const std::string& _filename);

	void Bind(unsigned int _slot = 0) const;

private:
	unsigned int m_ID;
};


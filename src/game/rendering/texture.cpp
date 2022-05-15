#include "texture.hpp"

#include <glad/glad.h>

#include <stb_image.h>

Texture::Texture() {
	glGenTextures(1, &m_ID);
}
Texture::~Texture() {
	glDeleteTextures(1, &m_ID);
}

void Texture::LoadFromData(unsigned char* _data, unsigned int _width, unsigned int _height) {
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA,  GL_UNSIGNED_BYTE, _data);
	// glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::LoadFromFile(const std::string& _filename) {
	int textureWidth, textureHeight, textureChans;
	unsigned char* textureData = stbi_load(_filename.c_str(), &textureWidth, &textureHeight, &textureChans, 4);
	LoadFromData(textureData, textureWidth, textureHeight);
	stbi_image_free(textureData);
}

void Texture::Bind(unsigned int _slot) const {
	glActiveTexture(GL_TEXTURE0 + _slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
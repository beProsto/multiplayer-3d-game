#include "mesh.hpp"
#include <glad/glad.h>

Mesh::Mesh() {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_IB);
	Bind();
}
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_IB);
	for(unsigned int& ab : m_ABs) {
		glDeleteBuffers(1, &ab);
	} 
}

void Mesh::Bind() const {
	glBindVertexArray(m_VAO);
}
unsigned int Mesh::GetIndexCount() const {
	return m_IndexCount;
}

void Mesh::SupplyIndices(const std::vector<unsigned int>& _data) {
	Bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*_data.size(), _data.data(), GL_STATIC_DRAW);
	m_IndexCount = _data.size();
}
void Mesh::SupplyArray(unsigned int _location, unsigned int _stride, const std::vector<float>& _data) {
	Bind();
	// m_ABs.push_back(VBO(GL_ARRAY_BUFFER));
	// m_ABs.back().Bind();
	// m_ABs.back().StoreData(_data);
	unsigned int arrayBuffer;
	glGenBuffers(1, &arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*_data.size(), _data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(_location);
	glVertexAttribPointer(_location, _stride, GL_FLOAT, false, _stride*sizeof(float), (void*)0);
	
	m_ABs.push_back(arrayBuffer);
}


void Mesh::Draw() const {
	Bind();
	glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, 0);
}
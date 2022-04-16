#include "mesh.hpp"

VBO::VBO() {
	m_ID = 0;
	m_Type= 0;
}
VBO::VBO(unsigned int _type) {
	Create(_type);
}
VBO::~VBO() {
	if(m_ID != 0 || m_Type != 0) {
		glDeleteBuffers(1, &m_ID);
	}
}
void VBO::Create(unsigned int _type) {
	m_Type = _type;
	glGenBuffers(1, &m_ID);
	Bind();
} 
void VBO::Bind() const {
	glBindBuffer(m_Type, m_ID);
}

Mesh::Mesh() {
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	m_IB.Create(GL_ELEMENT_ARRAY_BUFFER);
}
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_VAO);
}
void Mesh::SupplyIndices(const std::vector<unsigned int>& _data) {
	m_IB.StoreData(_data);
	m_IndexCount = _data.size();
}
unsigned int Mesh::GetIndexCount() const {
	return m_IndexCount;
}
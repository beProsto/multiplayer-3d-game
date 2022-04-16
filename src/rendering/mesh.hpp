#pragma once

#include <glad/glad.h>
#include <vector>

class VBO {
public:
	VBO();
	VBO(unsigned int _type);
	~VBO();

	void Create(unsigned int _type);

	template<typename T>
	void StoreData(const std::vector<T>& _data) {
		Bind();
		glBufferData(m_Type, _data.size() * sizeof(T), _data.data(), GL_STATIC_DRAW);
	}

	void Bind() const;

protected:
	unsigned int m_ID;
	unsigned int m_Type;
};

class Mesh {
public:
	Mesh();
	~Mesh();

	void SupplyIndices(const std::vector<unsigned int>& _data);

	template<typename T>
	void SupplyArray(unsigned int _location, unsigned int _stride, const std::vector<T>& _data) {
		m_ABs.push_back(VBO(GL_ARRAY_BUFFER));
		m_ABs.back().StoreData(_data);
		glVertexAttribPointer(_location, _stride, GL_FLOAT, false, _stride * sizeof(T), 0);
		glEnableVertexAttribArray(_location);
	}

protected:
	unsigned int m_VAO;
	VBO m_IB;
	std::vector<VBO> m_ABs;

	unsigned int m_IndexCount;
};

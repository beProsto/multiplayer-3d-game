#pragma once

#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh();

	void SupplyIndices(const std::vector<unsigned int>& _data);

	void SupplyArray(unsigned int _location, unsigned int _stride, const std::vector<float>& _data);

	unsigned int GetIndexCount() const;

	void Bind() const;

protected:
	unsigned int m_VAO;
	unsigned int m_IB;
	std::vector<unsigned int> m_ABs;

	unsigned int m_IndexCount;
};

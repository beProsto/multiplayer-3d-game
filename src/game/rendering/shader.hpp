#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "../../common/math/math.hpp"

class Shader {
public:
	Shader();
	~Shader();

	void LoadFromFile(const std::string& _filename);

	void Bind() const;

	void Set4x4f(const std::string& _name, const Math::Mat4& _v);
	void Set4f(const std::string& _name, const Math::Vec4& _v);
	void Set3f(const std::string& _name, const Math::Vec3& _v);
	void Set2f(const std::string& _name, const Math::Vec2& _v);
	void Set1f(const std::string& _name, float _v);

	int GetUniformLocation(const std::string& _name);
protected:
	unsigned int m_ID;
	std::unordered_map<std::string, int> m_UniformLocations;
};

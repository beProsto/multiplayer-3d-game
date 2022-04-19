#pragma once

const char* SHADER_BEGIN_BASIC = R"(#version 330
)"

class Shader {
public:
	Shader();
	~Shader();

	void Bind() const;

public:
	enum class Type {
		Basic = 0,
		Animated = 1,
	};

private:
	unsigned int m_ID;
};


class Material {
public:
	Material();
	~Material();

	void Bind() const;

protected:
	Type m_Type;
	Shader m_Shader;
};
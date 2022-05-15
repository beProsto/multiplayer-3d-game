#pragma once

#include "../../common/math/math.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "loaderOBJ.hpp"
#include "foreach.hpp"

#define DEF_ENUM_PAIR(element) \
	element,
#define DEF_ENUM_STRN(element) \
	case element : return #element;

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, ...) \
    enum name { \
        FOR_EACH(DEF_ENUM_PAIR, __VA_ARGS__) \
    }; \
	static std::string ShaderTypeToString(name v) \
    { \
        switch (v) \
        { \
            FOR_EACH(DEF_ENUM_STRN, __VA_ARGS__) \
            default: return "Unknown"; \
        } \
    }

struct Light {
	Math::Vec3 color;
	float ambient;
	Math::Vec3 direction;
};

class Renderer {
public:
	Renderer();
	~Renderer();

	void SetSceneLight(Light& _light);
	void SetSceneCamera(Camera& _camera);

	void BeginScene(uint32_t _width, uint32_t _height, float _aspect);

	void Draw(uint32_t _shader, const Mesh& _mesh, const Texture& _texture, const Math::Mat4& _transform);

public:
	DEFINE_ENUM_WITH_STRING_CONVERSIONS(ShaderType, 
		basic,
		inv,
		count
	)

protected:
	Shader m_Shaders[ShaderType::count];
	Light* m_Light;
	Camera* m_Camera;
};


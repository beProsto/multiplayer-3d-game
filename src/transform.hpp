#pragma once

#include "math/math.hpp"

class Transform {
public:
	Transform(Math::Vec3 _t = Math::Vec3(), Math::Vec3 _r = Math::Vec3(), Math::Vec3 _s = Math::Vec3(1.0f)) {
		Translation = _t;
		Rotation = _r;
		Scale = _s;
	}
	~Transform() {

	}

	Math::Mat4 GetMatrix() {
		return Math::Mat4::Translate(Translation.x, Translation.y, Translation.z) *
			Math::Mat4::RotateZ(Rotation.z) *
			Math::Mat4::RotateY(Rotation.y) *
			Math::Mat4::RotateX(Rotation.x) *
			Math::Mat4::Scale(Scale.x, Scale.y, Scale.z);
	}

public:
	Math::Vec3 Translation;
	Math::Vec3 Rotation;
	Math::Vec3 Scale;
	
};
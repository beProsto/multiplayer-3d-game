#pragma once

#include "math/math.hpp"

class Camera {
public:
	Camera(Math::Vec3 _translation = Math::Vec3(0.0f), Math::Vec3 _rotation = Math::Vec3(0.0f), float _fieldOfView = M_PI/2.0, float _aspectRatio = 16.0f/9.0f) {
		m_Translation = _translation;
		m_Rotation = _rotation;
		m_FieldOfView = _fieldOfView;
		m_AspectRatio = _aspectRatio;
		m_RecalcProj = true;
		m_RecalcView = true;
		RecalcMat();
	}
	~Camera() {

	}

	const Math::Mat4& GetMatrix() {
		RecalcMat();
		return m_Mat;
	}

	void SetTranslation(const Math::Vec3& _translation) {
		if(m_Translation != _translation) {
			m_RecalcView = true;
			m_Translation = _translation;
		}
	}
	const Math::Vec3& GetTranslation() {
		return m_Translation;
	}

	void SetRotation(const Math::Vec3& _rotation) {
		if(m_Rotation != _rotation) {
			m_RecalcView = true;
			m_Rotation = _rotation;
		}
	}
	const Math::Vec3& GetRotation() {
		return m_Rotation;
	}

	void SetAspectRatio(float _aspectRatio) {
		if(m_AspectRatio != _aspectRatio) {
			m_RecalcProj = true;
			m_AspectRatio = _aspectRatio;
		}
	}
	const float GetAspectRatio() {
		return m_AspectRatio;
	}

	void SetFieldOfView(float _fov) {
		if(m_FieldOfView != _fov) {
			m_RecalcProj = true;
			m_FieldOfView = _fov;
		}
	}
	const float GetFieldOfView() {
		return m_FieldOfView;
	}

private:
	void RecalcProj() {
		if(!m_RecalcProj) {
			return;
		}

		m_Proj = Math::Mat4::Perspective(m_FieldOfView, m_AspectRatio, 0.01f, 1000.0f);

		m_RecalcProj = false;

		// std::cout << "Camera: Proj Matrix recalc!\n";

	}
	void RecalcView() {
		if(!m_RecalcView) {
			return;
		}

		m_View = Math::Mat4::Inverse(
			Math::Mat4::Translate(m_Translation.x, m_Translation.y, m_Translation.z) *
			Math::Mat4::RotateZ(m_Rotation.z) *
			Math::Mat4::RotateY(m_Rotation.y) *
			Math::Mat4::RotateX(m_Rotation.x) 
		);

		m_RecalcView = false;

		// std::cout << "Camera: View Matrix recalc!\n";
	}

	void RecalcMat() {
		if(!m_RecalcView && !m_RecalcProj) {
			return;
		}

		RecalcProj();
		RecalcView();

		m_Mat = m_Proj * m_View;

		// std::cout << "Camera: Full Matrix recalc!\n";
	}

private:
	Math::Vec3 m_Translation;
	Math::Vec3 m_Rotation;

	Math::Mat4 m_View;
	Math::Mat4 m_Proj;

	Math::Mat4 m_Mat;

	float m_FieldOfView;
	float m_AspectRatio;

	bool m_RecalcView;
	bool m_RecalcProj;
};
#ifndef _ORE_VEC3_HPP_HEADER_FILE_GUARD
#define _ORE_VEC3_HPP_HEADER_FILE_GUARD

namespace Math {
	class Vec3 {
	public:
		Vec3(float _a = 0) {
			x = _a;
			y = _a;
			z = _a;
		}
		Vec3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
		Vec3(const Vec3& _other) {
			x = (_other.x);
			y = (_other.y);
			z = (_other.z);
		}

		static Vec3 Normalise(const Vec3& _v) {
			return _v / sqrtf(_v.x*_v.x + _v.y*_v.y + _v.z*_v.z);
		}

		float& operator[](size_t _i) {
			return *(((float*)this)+_i);
		}

		Vec3& operator=(const Vec3& _other) {
			x = (_other.x);
			y = (_other.y);
			z = (_other.z);
			return *this;
		}

		Vec3 operator+(const Vec3& _a) const {
			return Vec3(x + (_a.x), 
						y + (_a.y),
						z + (_a.z));
		}
		Vec3 operator-(const Vec3& _a) const {
			return Vec3(x - (_a.x), 
						y - (_a.y),
						z - (_a.z));
		}
		Vec3 operator*(const Vec3& _a) const {
			return Vec3(x * (_a.x), 
						y * (_a.y),
						z * (_a.z));
		}
		Vec3 operator/(const Vec3& _a) const {
			return Vec3(x / (_a.x), 
						y / (_a.y),
						z / (_a.z));
		}

		Vec3& operator+=(const Vec3& _a) {
			return (*this) = Vec3(x + (_a.x), 
									y + (_a.y),
									z + (_a.z));
		}
		Vec3& operator-=(const Vec3& _a) {
			return (*this) = Vec3(x - (_a.x), 
									y - (_a.y),
									z - (_a.z));
		}
		Vec3& operator*=(const Vec3& _a) {
			return (*this) = Vec3(x * (_a.x), 
									y * (_a.y),
									z * (_a.z));
		}
		Vec3& operator/=(const Vec3& _a) {
			return (*this) = Vec3(x / (_a.x), 
									y / (_a.y),
									z / (_a.z));
		}

		bool operator==(const Vec3& _a) const {
			return (x == (_a.x) && y == (_a.y) && z == (_a.z));
		}
		bool operator!=(const Vec3& _a) const {
			return (x != (_a.x) || y != (_a.y) || z != (_a.z));
		}
		bool operator<=(const Vec3& _a) const {
			return (x <= (_a.x) && y <= (_a.y) && z <= (_a.z));
		}
		bool operator>=(const Vec3& _a) const {
			return (x >= (_a.x) && y >= (_a.y) && z >= (_a.z));
		}
		bool operator<(const Vec3& _a) const {
			return (x < (_a.x) && y < (_a.y) && z < (_a.z));
		}
		bool operator>(const Vec3& _a) const {
			return (x > (_a.x) && y > (_a.y) && z > (_a.z));
		}
		

		union { /*[0]*/
			float x;
			float u;
			float r;	
		};
		union { /*[1]*/
			float y;
			float v;
			float g;
		};
		union { /*[2]*/
			float z;
			float b;
		};
	};

}

#endif/*_ORE_VEC3_HPP_HEADER_FILE_GUARD*/
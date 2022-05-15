#ifndef _ORE_VEC2_HPP_HEADER_FILE_GUARD
#define _ORE_VEC2_HPP_HEADER_FILE_GUARD

namespace Math {
	class Vec2 {
	public:
		Vec2(float _a = 0) {
			x = _a;
			y = _a;
		}
		Vec2(float _x, float _y) {
			x = _x;
			y = _y;
		}
		Vec2(const Vec2& _other) {
			x = (_other.x);
			y = (_other.y);
		}

		static Vec2 Normalise(const Vec2& _v) {
			return _v / sqrtf(_v.x*_v.x + _v.y*_v.y);
		}

		float& operator[](size_t _i) {
			return *(((float*)this)+_i);
		}

		Vec2& operator=(const Vec2& _other) {
			x = (_other.x);
			y = (_other.y);
			return *this;
		}

		Vec2 operator+(const Vec2& _a) const {
			return Vec2(x + (_a.x), 
						y + (_a.y));
		}
		Vec2 operator-(const Vec2& _a) const {
			return Vec2(x - (_a.x), 
						y - (_a.y));
		}
		Vec2 operator*(const Vec2& _a) const {
			return Vec2(x * (_a.x), 
						y * (_a.y));
		}
		Vec2 operator/(const Vec2& _a) const {
			return Vec2(x / (_a.x), 
						y / (_a.y));
		}

		Vec2& operator+=(const Vec2& _a) {
			return (*this) = Vec2(x + (_a.x), 
									y + (_a.y));
		}
		Vec2& operator-=(const Vec2& _a) {
			return (*this) = Vec2(x - (_a.x), 
									y - (_a.y));
		}
		Vec2& operator*=(const Vec2& _a) {
			return (*this) = Vec2(x * (_a.x), 
									y * (_a.y));
		}
		Vec2& operator/=(const Vec2& _a) {
			return (*this) = Vec2(x / (_a.x), 
									y / (_a.y));
		}

		bool operator==(const Vec2& _a) const {
			return (x == (_a.x) && y == (_a.y));
		}
		bool operator!=(const Vec2& _a) const {
			return (x != (_a.x) || y != (_a.y));
		}
		bool operator<=(const Vec2& _a) const {
			return (x <= (_a.x) && y <= (_a.y));
		}
		bool operator>=(const Vec2& _a) const {
			return (x >= (_a.x) && y >= (_a.y));
		}
		bool operator<(const Vec2& _a) const {
			return (x < (_a.x) && y < (_a.y));
		}
		bool operator>(const Vec2& _a) const {
			return (x > (_a.x) && y > (_a.y));
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
	};

}

#endif/*_ORE_VEC2_HPP_HEADER_FILE_GUARD*/
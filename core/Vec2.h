#pragma once
#include <cmath>

struct Vec2 {
	float x;
	float y;

	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
	Vec2 operator/(float s) const { return Vec2(x / s, y / s); }
	Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
	Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
	Vec2& operator*=(float s) { x *= s; y *= s; return *this; }
	Vec2& operator/=(float s) { x /= s; y /= s; return *this; }

	float length() const {
		return std::sqrt(x * x + y * y);
	}

	float lengthSquared() const {
		return x * x + y * y;
	}

	Vec2 normalized() const {
		float len = length();
		if (len < 0.00001f) return Vec2(0, 0);
		return Vec2(x / len, y / len);
	}

	void normalize() {
		float len = length();
		if (len < 0.00001f) return;
		x /= len;
		y /= len;
	}

	//iloczyn wektorowy
	float dot(const Vec2& v) const {
		return x * v.x + y * v.y;
	}

	//iloczyn skalarny
	float cross(const Vec2& v) const {
		return x * v.y - y * v.x;
	}
};

inline Vec2 operator*(float s, const Vec2& v) { return Vec2(v.x * s, v.y * s); }
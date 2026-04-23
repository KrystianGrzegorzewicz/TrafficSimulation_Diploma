#pragma once
#include <cmath>

struct Vec2 {
    float x;
    float y;

    // ================= KONSTRUKTORY =================
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    // ================= OPERATORY PODSTAWOWE =================
    Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(float s) const { return Vec2(x / s, y / s); }

    // ================= OPERATORY Z PRZYPISANIEM =================
    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2& operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2& operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
    }

    // ================= MATEMATYKA =================
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

    float dot(const Vec2& v) const {
        return x * v.x + y * v.y;
    }

    // 2D "cross" (zwraca skalar)
    float cross(const Vec2& v) const {
        return x * v.y - y * v.x;
    }
};

// ================= DODATKOWE =================

// żeby działało: 2.0f * vec
inline Vec2 operator*(float s, const Vec2& v) {
    return Vec2(v.x * s, v.y * s);
}
#ifndef GMATH_H
#define GMATH_H
#include <CMath>

namespace GUESS::core {
    // Vector2 Structures
    struct Vector2f {
        float x, y;

        Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

        Vector2f operator+(const Vector2f& other) const { return Vector2f(x + other.x, y + other.y); }
        Vector2f operator-(const Vector2f& other) const { return Vector2f(x - other.x, y - other.y); }
        Vector2f operator*(float scalar) const { return Vector2f(x * scalar, y * scalar); }
        float dot(const Vector2f& other) const { return x * other.x + y * other.y; }
        float magnitude() const { return std::sqrt(x * x + y * y); }
        Vector2f normalized() const { float mag = magnitude(); return Vector2f(x / mag, y / mag); }
    };

    // Vector3 Structures
    struct Vector3f {
        float x, y, z;

        Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

        Vector3f operator+(const Vector3f& other) const { return Vector3f(x + other.x, y + other.y, z + other.z); }
        Vector3f operator-(const Vector3f& other) const { return Vector3f(x - other.x, y - other.y, z - other.z); }
        Vector3f operator*(float scalar) const { return Vector3f(x * scalar, y * scalar, z * scalar); }
        float dot(const Vector3f& other) const { return x * other.x + y * other.y + z * other.z; }
        Vector3f cross(const Vector3f& other) const {
            return Vector3f(y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }
        float magnitude() const { return std::sqrt(x * x + y * y + z * z); }
        Vector3f normalized() const { float mag = magnitude(); return Vector3f(x / mag, y / mag, z / mag); }
    };

    // Matrix4x4 Structure
    struct Matrix4x4 {
        float m[4][4];

        Matrix4x4() { identity(); }

        void identity() {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        Matrix4x4 operator*(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++) {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 4; k++)
                        result.m[i][j] += m[i][k] * other.m[k][j];
                }
            return result;
        }

        static Matrix4x4 translation(float x, float y, float z) {
            Matrix4x4 result;
            result.m[0][3] = x;
            result.m[1][3] = y;
            result.m[2][3] = z;
            return result;
        }

        static Matrix4x4 rotation(float angle, float x, float y, float z) {
            Matrix4x4 result;
            float c = cos(angle);
            float s = sin(angle);
            float t = 1.0f - c;

            // Normalize rotation axis
            float len = sqrt(x * x + y * y + z * z);
            x /= len;
            y /= len;
            z /= len;

            result.m[0][0] = t * x * x + c;
            result.m[0][1] = t * x * y - s * z;
            result.m[0][2] = t * x * z + s * y;

            result.m[1][0] = t * x * y + s * z;
            result.m[1][1] = t * y * y + c;
            result.m[1][2] = t * y * z - s * x;

            result.m[2][0] = t * x * z - s * y;
            result.m[2][1] = t * y * z + s * x;
            result.m[2][2] = t * z * z + c;

            return result;
        }

        static Matrix4x4 scale(float x, float y, float z) {
            Matrix4x4 result;
            result.m[0][0] = x;
            result.m[1][1] = y;
            result.m[2][2] = z;
            return result;
        }

        static Matrix4x4 perspective(float fov, float aspect, float near, float far) {
            Matrix4x4 result;
            float tanHalfFov = tan(fov / 2.0f);

            result.m[0][0] = 1.0f / (aspect * tanHalfFov);
            result.m[1][1] = 1.0f / tanHalfFov;
            result.m[2][2] = -(far + near) / (far - near);
            result.m[2][3] = -(2.0f * far * near) / (far - near);
            result.m[3][2] = -1.0f;
            result.m[3][3] = 0.0f;

            return result;
        }

    };

    // Quaternion Structure
    struct Quaternion {
        float w, x, y, z;

        Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : w(w), x(x), y(y), z(z) {}

        Quaternion operator*(const Quaternion& other) const {
            return Quaternion(
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
            );
        }

        void normalize() {
            float mag = std::sqrt(w * w + x * x + y * y + z * z);
            w /= mag; x /= mag; y /= mag; z /= mag;
        }

        static Quaternion fromEuler(float pitch, float yaw, float roll);
        Matrix4x4 toMatrix() const;
    };

    // Additional utility functions
    const float PI = 3.14159265359f;
    float toRadians(float degrees);
    float toDegrees(float radians);
    float lerp(float a, float b, float t);
    Vector3f lerp(const Vector3f& a, const Vector3f& b, float t);
    Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);
}

#endif GMATH_H
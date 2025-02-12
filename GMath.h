#ifndef GMATH_H
#define GMATH_H
#include <intrin.h>
#include <chrono>
#include <thread>

namespace GUESS::core::math {
    template<typename T>
    struct Vector2 {
        T x, y;

        Vector2(T x = 0, T y = 0) : x(x), y(y) {}

        Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
        Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
        Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
        T dot(const Vector2<T>& other) const { return x * other.x + y * other.y; }
        T magnitude() const { return std::sqrt(x * x + y * y); }
        Vector2<T> normalized() const { T mag = magnitude(); return Vector2<T>(x / mag, y / mag); }
    };

    template<typename T>
    struct Vector3 {
        T x, y, z;

        Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

        Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y + other.y, z + other.z); }
        Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(x - other.x, y - other.y, z - other.z); }
        Vector3<T> operator*(T scalar) const { return Vector3<T>(x * scalar, y * scalar, z * scalar); }
        T dot(const Vector3<T>& other) const { return x * other.x + y * other.y + z * other.z; }
        Vector3<T> cross(const Vector3<T>& other) const {
            return Vector3<T>(y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
        }
        T magnitude() const { return std::sqrt(x * x + y * y + z * z); }
        Vector3<T> normalized() const { T mag = magnitude(); return Vector3<T>(x / mag, y / mag, z / mag); }
    };

    using Vector2i = Vector2<int>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;

    using Vector3i = Vector3<int>;
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;

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
            float mag = sqrt(w * w + x * x + y * y + z * z);
            w /= mag; x /= mag; y /= mag; z /= mag;
        }

        static Quaternion fromEuler(float pitch, float yaw, float roll);
        Matrix4x4 toMatrix() const;
    };

    // Ray structure for raycasting
    struct Ray {
        Vector3f origin;
        Vector3f direction;

        Ray(const Vector3f& o, const Vector3f& d) : origin(o), direction(d.normalized()) {}

        Vector3f getPoint(float t) const {
            return origin + direction * t;
        }
    };

    // Plane structure
    struct Plane {
        Vector3f normal;
        float distance;

        Plane(const Vector3f& n = Vector3f(0, 1, 0), float d = 0) : normal(n.normalized()), distance(d) {}

        float getSignedDistance(const Vector3f& point) const {
            return normal.dot(point) + distance;
        }
    };

    // AABB structure
    struct AABB {
        Vector3f min;
        Vector3f max;

        AABB(const Vector3f& min = Vector3f(), const Vector3f& max = Vector3f())
            : min(min), max(max) {}

        bool contains(const Vector3f& point) const {
            return point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y &&
                point.z >= min.z && point.z <= max.z;
        }
    };

    // Frustrum structure
    struct Frustum {
        Plane planes[6]; // Left, Right, Bottom, Top, Near, Far

        Frustum() = default;

        void updateFromMatrix(const Matrix4x4& viewProjection) {
            // Extract planes from view-projection matrix
            // Left plane
            planes[0].normal.x = viewProjection.m[3][0] + viewProjection.m[0][0];
            planes[0].normal.y = viewProjection.m[3][1] + viewProjection.m[0][1];
            planes[0].normal.z = viewProjection.m[3][2] + viewProjection.m[0][2];
            planes[0].distance = viewProjection.m[3][3] + viewProjection.m[0][3];

            // Right plane
            planes[1].normal.x = viewProjection.m[3][0] - viewProjection.m[0][0];
            planes[1].normal.y = viewProjection.m[3][1] - viewProjection.m[0][1];
            planes[1].normal.z = viewProjection.m[3][2] - viewProjection.m[0][2];
            planes[1].distance = viewProjection.m[3][3] - viewProjection.m[0][3];

            // Bottom plane
            planes[2].normal.x = viewProjection.m[3][0] + viewProjection.m[1][0];
            planes[2].normal.y = viewProjection.m[3][1] + viewProjection.m[1][1];
            planes[2].normal.z = viewProjection.m[3][2] + viewProjection.m[1][2];
            planes[2].distance = viewProjection.m[3][3] + viewProjection.m[1][3];

            // Top plane
            planes[3].normal.x = viewProjection.m[3][0] - viewProjection.m[1][0];
            planes[3].normal.y = viewProjection.m[3][1] - viewProjection.m[1][1];
            planes[3].normal.z = viewProjection.m[3][2] - viewProjection.m[1][2];
            planes[3].distance = viewProjection.m[3][3] - viewProjection.m[1][3];

            // Near plane
            planes[4].normal.x = viewProjection.m[3][0] + viewProjection.m[2][0];
            planes[4].normal.y = viewProjection.m[3][1] + viewProjection.m[2][1];
            planes[4].normal.z = viewProjection.m[3][2] + viewProjection.m[2][2];
            planes[4].distance = viewProjection.m[3][3] + viewProjection.m[2][3];

            // Far plane
            planes[5].normal.x = viewProjection.m[3][0] - viewProjection.m[2][0];
            planes[5].normal.y = viewProjection.m[3][1] - viewProjection.m[2][1];
            planes[5].normal.z = viewProjection.m[3][2] - viewProjection.m[2][2];
            planes[5].distance = viewProjection.m[3][3] - viewProjection.m[2][3];

            // Normalize all planes
            for (int i = 0; i < 6; i++) {
                float length = planes[i].normal.magnitude();
                planes[i].normal = planes[i].normal * (1.0f / length);
                planes[i].distance /= length;
            }
        }

        bool isPointVisible(const Vector3f& point) const {
            for (int i = 0; i < 6; i++) {
                if (planes[i].getSignedDistance(point) < 0) {
                    return false;
                }
            }
            return true;
        }

        bool isSphereVisible(const Vector3f& center, float radius) const {
            for (int i = 0; i < 6; i++) {
                if (planes[i].getSignedDistance(center) < -radius) {
                    return false;
                }
            }
            return true;
        }

        bool isAABBVisible(const AABB& aabb) const {
            for (int i = 0; i < 6; i++) {
                Vector3f positive = aabb.min;
                if (planes[i].normal.x >= 0) positive.x = aabb.max.x;
                if (planes[i].normal.y >= 0) positive.y = aabb.max.y;
                if (planes[i].normal.z >= 0) positive.z = aabb.max.z;

                if (planes[i].getSignedDistance(positive) < 0) {
                    return false;
                }
            }
            return true;
        }
    };

    // Extended Vector3f operations
    inline Vector3f reflect(const Vector3f & v, const Vector3f & normal) {
        return v - normal * 2.0f * v.dot(normal);
    }

    inline Vector3f project(const Vector3f & v, const Vector3f & onto) {
        return onto * (v.dot(onto) / onto.dot(onto));
    }

    // Extended Matrix4x4 operations
    inline Matrix4x4 transpose(const Matrix4x4 & m) {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[i][j] = m.m[j][i];
        return result;
    }

    inline Matrix4x4 Quaternion::toMatrix() const {
        Matrix4x4 result;
        
        // Calculate matrix elements
        result.m[0][0] = 1.0f - 2.0f * (y * y + z * z);
        result.m[0][1] = 2.0f * (x * y - w * z);
        result.m[0][2] = 2.0f * (x * z + w * y);
        result.m[0][3] = 0.0f;
    
        result.m[1][0] = 2.0f * (x * y + w * z);
        result.m[1][1] = 1.0f - 2.0f * (x * x + z * z);
        result.m[1][2] = 2.0f * (y * z - w * x);
        result.m[1][3] = 0.0f;
    
        result.m[2][0] = 2.0f * (x * z - w * y);
        result.m[2][1] = 2.0f * (y * z + w * x);
        result.m[2][2] = 1.0f - 2.0f * (x * x + y * y);
        result.m[2][3] = 0.0f;
    
        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;
    
        return result;
    }

    // View matrix generation
    static Matrix4x4 lookAt(const Vector3f & eye, const Vector3f & target, const Vector3f & up) {
        Matrix4x4 result;

        Vector3f f = (target - eye).normalized();
        Vector3f s = f.cross(up).normalized();
        Vector3f u = s.cross(f);

        result.m[0][0] = s.x;  result.m[0][1] = s.y;  result.m[0][2] = s.z;
        result.m[1][0] = u.x;  result.m[1][1] = u.y;  result.m[1][2] = u.z;
        result.m[2][0] = -f.x; result.m[2][1] = -f.y; result.m[2][2] = -f.z;

        result.m[0][3] = -s.dot(eye);
        result.m[1][3] = -u.dot(eye);
        result.m[2][3] = f.dot(eye);

        return result;
    }

    // Curve mathematics
    inline Vector3f bezierCurve(const Vector3f & p0, const Vector3f & p1, const Vector3f & p2, float t) {
        float oneMinusT = 1.0f - t;
        return p0 * (oneMinusT * oneMinusT) +
            p1 * (2.0f * oneMinusT * t) +
            p2 * (t * t);
    }

    // Easing functions
    inline float easeInQuad(float t) {
        return t * t;
    }

    inline float easeOutQuad(float t) {
        return t * (2.0f - t);
    }


    // Additional utility functions
    constexpr float PI = 3.14159265359f;
    float toRadians(float degrees);
    float toDegrees(float radians);
    float lerp(float a, float b, float t);
    Vector3f lerp(const Vector3f& a, const Vector3f& b, float t);
    Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);
    inline float factorial(int n) {
        if (n <= 1) return 1.0f;
        return n * factorial(n - 1);
    }
    inline float pow(float base, int exponent) {
        float result = 1.0f;
        int sign = (base < 0 && exponent % 2) ? -1 : 1;
        if (base < 0)
            base *= -1;
        int exponMag = exponent;
        if (exponent < 0)
            return 1.0f / (pow(base, -exponent));
        while (exponMag > 0) {
            if (exponMag & 1) {
                result *= base;
            }
            base *= base;
            exponMag >>= 1;
        }
        return sign * result;
    }
    inline float fractionalPow(float base, int numerator, int denominator) {
        if (numerator < 0 && denominator < 0) {
            numerator *= -1;
            denominator *= -1;
        }
        if (numerator < 0 || denominator < 0)
            return 1/nthrt(pow(base, numerator), denominator);

        return nthrt(pow(base, numerator), denominator);
    }
    inline float taylorSeries(float x, float center, int iterations) {
        if (iterations == 0) {
            return 1.0f;
        }

        float term = pow(x - center, iterations) / factorial(iterations);
        return term + taylorSeries(x, center, iterations - 1);
    }
    inline float sqrt(float n) {
        return 1 / fisr(n);
    }
    // I love stealing 
    inline float fisr(float n) {
        const float threehalfs = 1.5F;
        float y = n;

        long i = *(long*)&y;

        i = 0x5f3759df - (i >> 1);
        y = *(float*)&i;

        y = y * (threehalfs - ((n * 0.5F) * y * y));

        return y;
    }
    inline float nthrt(float x, int n) {

        if (n <= 0) return 0; 
        if (x == 0) return 0; 
        if (x < 0 && n % 2 == 0) return 0; 
    
        int sign = (x < 0) ? -1 : 1;
        if (x < 0) x = -x; 
    
        // Bitwise magic: Adjust exponent
        float y = x;
        int i = *(int*)&y; 
        i = (i - (127 << 23)) / n + (127 << 23); 
        y = *(float*)&i; 

        return sign * y;
    }
    // trig functions
    // sin, cos, tan taking the sides
    inline float sin(float opp, float hyp) { if (hyp == 0) { return 0.0; } return opp / hyp; }
    inline float cos(float adj, float hyp) { if (hyp == 0) { return 0.0; }return adj / hyp; }
    inline float tan(float opp, float adj) { if (adj == 0) { return 0.0; }return opp / adj; }
   
    // sin, cos, tan taking the angle (in radians)
    // sin using taylor series
    inline float sin(float radians) {

        return taylorSeries(radians, 0.0f, 20);
    }
    inline float cos(float radians) {
        return sin((PI / 2) - radians);
    }
    inline float tan(float radians) {
        float cosValue = cos(radians);
        if (cosValue == 0.0f) return 0.0f; 
        return sin(radians) / cosValue;
    }
    
    // arcsin using taylor series
    inline float arcsin(float x) {
        if (x > 1.0 || x < -1.0) return 0.0;

        return taylorSeries(x, 0.0f, 20);
    }
    inline float arccos(float x) {
        if (x > 1.0 || x < -1.0) return 0.0;
        return PI / 2.0 - arcsin(x);
    }
    // arctan using Taylor series
    inline float arctan(float x) {
        if (x > 1.0 || x < -1.0) {
            x = 1.0 / x;
            return (x > 0 ? -PI / 2.0 : PI / 2.0) - taylorSeries(x, 0.0f, 40);
        }
        return taylorSeries(x, 0.0f, 20);
    }


    //
    // Randomization
    // Random numbers don't exist but hopefully this is close enough
    //
    // Noise function (simple implementation)
    inline float noise1D(int x) {
        x = (x << 13) ^ x;
        return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

    // Permutation table for noise generation don't look at it, its gross
    static const int PERM[512] = {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,1428,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

    inline float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    inline float grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    // 3D Perlin Noise
    inline float perlinNoise(float x, float y, float z) {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;
        int Z = (int)floor(z) & 255;

        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        int A = PERM[X] + Y;
        int AA = PERM[A] + Z;
        int AB = PERM[A + 1] + Z;
        int B = PERM[X + 1] + Y;
        int BA = PERM[B] + Z;
        int BB = PERM[B + 1] + Z;

        return lerp(
            lerp(
                lerp(grad(PERM[AA], x, y, z),
                    grad(PERM[BA], x - 1, y, z),
                    u),
                lerp(grad(PERM[AB], x, y - 1, z),
                    grad(PERM[BB], x - 1, y - 1, z),
                    u),
                v),
            lerp(
                lerp(grad(PERM[AA + 1], x, y, z - 1),
                    grad(PERM[BA + 1], x - 1, y, z - 1),
                    u),
                lerp(grad(PERM[AB + 1], x, y - 1, z - 1),
                    grad(PERM[BB + 1], x - 1, y - 1, z - 1),
                    u),
                v),
            w);
    }

    // Simplex Noise constants
    static const float F3 = 1.0f / 3.0f;
    static const float G3 = 1.0f / 6.0f;

    // 3D Simplex Noise
    inline float simplexNoise(float x, float y, float z) {
        float s = (x + y + z) * F3;
        int i = floor(x + s);
        int j = floor(y + s);
        int k = floor(z + s);

        float t = (i + j + k) * G3;
        float X0 = i - t;
        float Y0 = j - t;
        float Z0 = k - t;

        float x0 = x - X0;
        float y0 = y - Y0;
        float z0 = z - Z0;

        int i1, j1, k1;
        int i2, j2, k2;

        if (x0 >= y0) {
            if (y0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
            else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; }
            else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; }
        }
        else {
            if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; }
            else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; }
            else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
        }

        float x1 = x0 - i1 + G3;
        float y1 = y0 - j1 + G3;
        float z1 = z0 - k1 + G3;
        float x2 = x0 - i2 + 2.0f * G3;
        float y2 = y0 - j2 + 2.0f * G3;
        float z2 = z0 - k2 + 2.0f * G3;
        float x3 = x0 - 1.0f + 3.0f * G3;
        float y3 = y0 - 1.0f + 3.0f * G3;
        float z3 = z0 - 1.0f + 3.0f * G3;

        int ii = i & 255;
        int jj = j & 255;
        int kk = k & 255;

        float n0, n1, n2, n3;

        float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
        if (t0 < 0.0f) n0 = 0.0f;
        else {
            t0 *= t0;
            n0 = t0 * t0 * grad(PERM[ii + PERM[jj + PERM[kk]]], x0, y0, z0);
        }

        float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
        if (t1 < 0.0f) n1 = 0.0f;
        else {
            t1 *= t1;
            n1 = t1 * t1 * grad(PERM[ii + i1 + PERM[jj + j1 + PERM[kk + k1]]], x1, y1, z1);
        }

        float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
        if (t2 < 0.0f) n2 = 0.0f;
        else {
            t2 *= t2;
            n2 = t2 * t2 * grad(PERM[ii + i2 + PERM[jj + j2 + PERM[kk + k2]]], x2, y2, z2);
        }

        float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
        if (t3 < 0.0f) n3 = 0.0f;
        else {
            t3 *= t3;
            n3 = t3 * t3 * grad(PERM[ii + 1 + PERM[jj + 1 + PERM[kk + 1]]], x3, y3, z3);
        }

        return 32.0f * (n0 + n1 + n2 + n3);
    }

    struct RandomState {
        uint64_t seed;
        uint64_t state;

        static uint64_t generateHardwareSeed() {
            uint64_t seed = 0;

            // Mix in system time
            auto now = std::chrono::high_resolution_clock::now();
            uint64_t time = now.time_since_epoch().count();
            seed ^= time;

            // Mix in memory address entropy
            void* addr = malloc(1);
            seed ^= reinterpret_cast<uint64_t>(addr);
            free(addr);

            // Mix in thread ID
            seed ^= std::hash<std::thread::id>{}(std::this_thread::get_id());

            // CPU cycle count if available
#if defined(_MSC_VER)
            seed ^= __rdtsc();
#elif defined(__GNUC__)
            uint32_t lo, hi;
            __asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
            seed ^= ((uint64_t)hi << 32) | lo;
#endif

            return seed;
        }

        RandomState() : seed(generateHardwareSeed()), state(seed) {}
        RandomState(uint64_t initial_seed) : seed(initial_seed), state(initial_seed) {}
    };

    class Random {
    private:
        RandomState state;

        // Xorshift64* algorithm
        uint64_t xorshift64() {
            state.state ^= state.state >> 12;
            state.state ^= state.state << 25;
            state.state ^= state.state >> 27;
            return state.state * 0x2545F4914F6CDD1DULL;
        }

        // Linear Congruential Generator
        uint64_t lcg() {
            state.state = state.state * 6364136223846793005ULL + 1;
            return state.state;
        }

    public:
        Random() : state() {}
        Random(uint64_t seed) : state(seed) {}

        // Generate integer in range [0, max)
        uint64_t nextInt(uint64_t max) {
            return xorshift64() % max;
        }

        // Generate integer in range [min, max)
        uint64_t nextInt(uint64_t min, uint64_t max) {
            return min + (xorshift64() % (max - min));
        }

        // Generate float in range [0, 1)
        float nextFloat() {
            return (xorshift64() >> 11) * (1.0f / (float)(1ULL << 53));
        }

        // Generate float in range [min, max)
        float nextFloat(float min, float max) {
            return min + nextFloat() * (max - min);
        }

        // Generate normalized Vector2f
        Vector2f nextVector2() {
            float angle = nextFloat() * 2.0f * PI;
            return Vector2f(cos(angle), sin(angle));
        }

        // Generate random Vector2f in rectangle
        Vector2f nextVector2(float minX, float maxX, float minY, float maxY) {
            return Vector2f(
                nextFloat(minX, maxX),
                nextFloat(minY, maxY)
            );
        }

        // Generate normalized Vector3f
        Vector3f nextVector3() {
            float theta = nextFloat() * 2.0f * PI;
            float phi = arccos(2.0f * nextFloat() - 1.0f);
            float r = pow(nextFloat(), 1.0f / 3.0f);

            return Vector3f(
                r * sin(phi) * cos(theta),
                r * sin(phi) * sin(theta),
                r * cos(phi)
            );
        }

        // Generate random Vector3f in box
        Vector3f nextVector3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
            return Vector3f(
                nextFloat(minX, maxX),
                nextFloat(minY, maxY),
                nextFloat(minZ, maxZ)
            );
        }

        // Generate random rotation quaternion
        Quaternion nextRotation() {
            float x = nextFloat();
            float r = sqrt(1.0f - x);
            float theta = nextFloat() * 2.0f * PI;
            float phi = nextFloat() * 2.0f * PI;

            return Quaternion(
                sqrt(x),
                r * sin(theta) * cos(phi),
                r * sin(theta) * sin(phi),
                r * cos(theta)
            );
        }
    };
}

#endif GMATH_H
#include <iostream>
#include <cmath> 
class Vector3f {
public:
    // Constructors
    Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    // Setter methods
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setZ(float newZ) { z = newZ; }

    // Additional methods
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Overloaded operators
    Vector3f operator+(const Vector3f& other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }

    Vector3f operator-(const Vector3f& other) const {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }

    // Dot product
    float dot(const Vector3f& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

private:
    float x, y, z;
};

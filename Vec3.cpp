#include "Vec3.h"

#include <cmath>
#include <iostream>

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3::~Vec3() {}

// Vektoraddition
Vec3 Vec3::add(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}
Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}
Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

// Vektorsubtraktion
Vec3 Vec3::subtract(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}
Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}
Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

// Skalierung
Vec3 Vec3::scale(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}
Vec3 Vec3::operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

// Berechnung der Länge (Betrag) des Vektors
float Vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Normierung (Einhaltung der Länge = 1)
Vec3 Vec3::normalize() const {
    float len = length();
    if (len > 0) {
        return scale(1.0f / len);
    }
    return *this; // Wenn der Vektor eine Länge von 0 hat, bleibt er unverändert
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

// Ausgabe des Vektors
void Vec3::print() const {
    std::cout << "Vec3(" << x << ", " << y << ", " << z << ")";//\n";
}

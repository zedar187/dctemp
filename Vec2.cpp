#include "Vec2.h"

#include <cmath>
#include <iostream>

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::~Vec2() {}

// Vektoraddition
Vec2 Vec2::add(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}
Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// Vektorsubtraktion
Vec2 Vec2::subtract(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}
Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// Skalierung
Vec2 Vec2::scale(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

// Berechnung der Länge (Betrag) des Vektors
float Vec2::length() const {
    return std::sqrt(x * x + y * y);
}

// Normierung (Einhaltung der Länge = 1)
Vec2 Vec2::normalize() const {
    float len = length();
    if (len > 0) {
        return scale(1.0f / len);
    }
    return *this; // Wenn der Vektor eine Länge von 0 hat, bleibt er unverändert
}

// Ausgabe des Vektors
void Vec2::print() const {
    std::cout << "Vec2(" << x << ", " << y << ", " << ")";//\n";
}

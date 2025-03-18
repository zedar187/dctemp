#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <cmath>
#include <iostream>

class Vec2 {
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);
    ~Vec2();

    Vec2 add(const Vec2& other) const;
    Vec2 operator+(const Vec2& other) const;
    Vec2& operator+=(const Vec2& other);

    Vec2 subtract(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2& operator-=(const Vec2& other);

    // Skalierung
    Vec2 scale(float scalar) const;
    Vec2 operator*(float scalar) const;

    // Berechnung der Länge (Betrag) des Vektors
    float length() const;

    // Normierung (Einhaltung der Länge = 1)
    Vec2 normalize() const;

    // Ausgabe des Vektors
    void print() const;
};

#endif //__VECTOR2_H__

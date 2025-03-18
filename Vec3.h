#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cmath>
#include <iostream>

class Vec3 {
public:
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);
    ~Vec3();

    Vec3 add(const Vec3& other) const;
    Vec3 operator+(const Vec3& other) const;
    Vec3& operator+=(const Vec3& other);

    Vec3 subtract(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);

    // Skalierung
    Vec3 scale(float scalar) const;
    Vec3 operator*(float scalar) const;

    // Berechnung der Länge (Betrag) des Vektors
    float length() const;

    // Normierung (Einhaltung der Länge = 1)
    Vec3 normalize() const;

    // Kreuzprodukt
    Vec3 cross(const Vec3& other) const;

    // Ausgabe des Vektors
    void print() const;
};

#endif //__VECTOR3_H__

// Material.h
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <vector>
#include <memory>
#include <array>
#include <GL/gl.h>

class Material {
public:
    std::string name;  // Name des Materials
    std::array<float, 4> ambient;  // Umgebungslichtfarbe (Ka)
    std::array<float, 4> diffuse;  // Diffuse Farbe (Kd)
    std::array<float, 4> specular; // Glanzlichtfarbe (Ks)
    float shininess;   // Glanzexponent (Ns)
    std::string textureFile;  // Pfad zur Diffuse-Textur
    GLuint textureID;         // Geladene OpenGL-Textur-ID

    Material();
};

#endif // __MATERIAL_H__
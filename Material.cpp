#include "Material.h"

Material::Material()
    : shininess(0.0f), textureID(0) {
    ambient[0] = ambient[1] = ambient[2] = 0.0f; ambient[3] = 1.0f;
    diffuse[0] = diffuse[1] = diffuse[2] = 0.0f; diffuse[3] = 1.0f;
    specular[0] = specular[1] = specular[2] = 0.0f; specular[3] = 1.0f;
}

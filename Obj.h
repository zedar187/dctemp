#ifndef __OBJ_H__
#define __OBJ_H__

#include <vector>
#include <memory>
#include <string>
#include "Vec2.h"
#include "Vec3.h"
#include "Material.h"

class Obj {
public:
    // Geometriedaten
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<unsigned int> materialIndices;

    // Indizes
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> normalIndices;
    std::vector<unsigned int> uvIndices;

    // Materialien
    std::vector<std::shared_ptr<Material>> materials;  // Materialien für die Faces
    std::shared_ptr<Material> currentMaterial;         // Aktuell verwendetes Material
    int currentMaterialIndex = -1;

    // Hilfsfunktionen
    size_t numFaces() const {
        return vertexIndices.size() / 3;  // Ein Face besteht immer aus 3 Vertices
    }

    void debugMaterials() const {
        std::cout << "Materials im Obj:" << std::endl;
        for (auto& mat : materials) {
            std::cout << "- " << mat->name << std::endl;
        }

        std::cout << "MaterialIndex im Obj:" << std::endl;
        for (auto& mat : materialIndices) {
            std::cout << "- " << mat << std::endl;
        }
    }
};

#endif //__OBJ_H__
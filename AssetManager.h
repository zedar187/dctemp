#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include <GL/gl.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "Obj.h"
#include "Material.h"

class AssetManager {
public:
    AssetManager();
    ~AssetManager();
    std::shared_ptr<Obj> loadObjModel(const std::string& filename);
    std::shared_ptr<Material> loadMaterial(const std::string& materialName);
    void printModelsInCache();
    void printMaterialsInCache();
private:
    Obj loadObjFromFile(const std::string& filename);
    void parseVertex(std::istringstream& ss, Obj& obj);
    void parseNormal(std::istringstream& ss, Obj& obj);
    void parseTexture(std::istringstream& ss, Obj& obj);
    void parseFace(std::istringstream& ss, Obj& obj, bool hasTexture);

    void parseMtlFile(const std::string& filePath);

    std::unordered_map<std::string, std::shared_ptr<Obj>> m_models;
    std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
};

#endif //__ASSETMANAGER_H__
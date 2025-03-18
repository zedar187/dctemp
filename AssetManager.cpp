#include "AssetManager.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/gl.h>
#include <unordered_map>
#include <iterator>

#include "Vec2.h"
#include "Vec3.h"

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}


std::shared_ptr<Obj> AssetManager::loadObjModel(const std::string& filename) {
    // Model bereits gecached?
    auto it = m_models.find(filename);
    if (it != m_models.end()) {
        return it->second;
    }

    std::cout << "First read of model " << filename << std::endl;
    Obj obj = loadObjFromFile(filename);
    if (obj.numFaces() > 0) {
        m_models[filename] = std::make_shared<Obj>(obj); // Model im Cache speichern
    } else {
        //TODO: Errorhandling fehlt
    }

    return std::make_shared<Obj>(obj);
}

std::shared_ptr<Material> AssetManager::loadMaterial(const std::string &materialName) {
    auto it = m_materials.find(materialName);
    if (it != m_materials.end()) {
        return it->second;
    }

    std::cerr << "Material '" << materialName << "' nicht gefunden, Erstellen eines Standardmaterials." << std::endl;
    auto defaultMaterial = std::make_shared<Material>();
    defaultMaterial->name = materialName;
    defaultMaterial->diffuse[0] = 1.0f;
    defaultMaterial->diffuse[1] = 1.0f;
    defaultMaterial->diffuse[2] = 1.0f;
    m_materials[materialName] = defaultMaterial;

    return defaultMaterial;
}

void AssetManager::printModelsInCache() {
    std::cout << "Models in Cache:" << std::endl;
    for (auto& model : m_models) {
        std::cout << "- " << model.first << std::endl;
    }
}

void AssetManager::printMaterialsInCache() {
    std::cout << "Materials in Cache:" << std::endl;
    for (auto& mtl : m_materials) {
        std::cout << "- " << mtl.first << std::endl;
    }
}

Obj AssetManager::loadObjFromFile(const std::string& filename) {
    Obj obj;

    std::ifstream file("/rd/" + filename);
    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: /rd/" << filename << std::endl;
        return obj;
    }

    std::string line;
    std::string materialFile;
    std::shared_ptr<Material> currentMaterial;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        size_t firstNonWhitespace = line.find_first_not_of(" \t");

        if (firstNonWhitespace != std::string::npos) {
            line = line.substr(firstNonWhitespace);
        } else {
            line.clear();
        }

        std::istringstream ss(line);
        std::string code;
        ss >> code;

        if (code == "v") {         // Vertex
            parseVertex(ss, obj);
        } else if (code == "vn") {   // Normal
            parseNormal(ss, obj);
        } else if (code == "vt") {   // Texturkoordinaten
            parseTexture(ss, obj);
        } else if (code == "f") {    // Face
            parseFace(ss, obj, !obj.uvs.empty());
        } else if (code == "mtllib") {
            // Materialbibliothek laden
            ss >> materialFile;
            std::cout << "AssetManager: mtllib erkannt." << std::endl;
            std::cout << "MaterialFile ausgelesen: " << materialFile << std::endl;
            parseMtlFile("/rd/" + materialFile);
        } else if (code == "usemtl") {
            std::string materialName;
            ss >> materialName;
            std::cout << "AssetManager: usemtl erkannt." << std::endl;
            std::cout << "MTL Wechsel: " << materialName << std::endl;
            auto tempmaterial = loadMaterial("/rd/" + materialFile +"."+ materialName);

            obj.currentMaterialIndex = static_cast<int>(obj.materials.size()); 
            obj.materials.push_back(tempmaterial);
            obj.currentMaterial = tempmaterial;
        }
    }

    file.close();
    obj.debugMaterials();
    return obj;
}

void AssetManager::parseVertex(std::istringstream& ss, Obj& obj) {
    Vec3 vertex;
    ss >> vertex.x >> vertex.y >> vertex.z;
    obj.vertices.push_back(vertex);
}

void AssetManager::parseNormal(std::istringstream& ss, Obj& obj) {
    Vec3 normal;
    ss >> normal.x >> normal.y >> normal.z;
    obj.normals.push_back(normal);
}

void AssetManager::parseTexture(std::istringstream& ss, Obj& obj) {
    Vec2 uv;
    ss >> uv.x >> uv.y;
    obj.uvs.push_back(uv);
}

void AssetManager::parseFace(std::istringstream& ss, Obj& obj, bool hasTexture) {
    std::string vertex;
    while (ss >> vertex) {
        // std::cerr << "AssetManager: " << std::endl;
        // std::cerr << "-- vertex: " << vertex << std::endl;
        // std::cerr << "-- Obj.currentmaterial: " << obj.currentMaterial->name << std::endl;

        // Format: vertex/uv/normal oder vertex//normal (keine Texturkoordinaten)
        int slashCount = std::count(vertex.begin(), vertex.end(), '/');
        
        if (slashCount == 0) {
            // Nur ein Vertex, ohne Texturkoordinaten und Normalen
            try {
                int vertexIndex = std::stoi(vertex);
                obj.vertexIndices.push_back(vertexIndex);
                obj.uvIndices.push_back(0);  // Kein Texturindex
                obj.normalIndices.push_back(0);   // Kein Normalenindex
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ungültiger Vertex Index: " << vertex << std::endl;
                return;
            } catch (const std::out_of_range& e) {
                std::cerr << "Index außerhalb des gültigen Bereichs: " << vertex << std::endl;
                return;
            }
        }
        else if (slashCount == 1) {
            // Format: vertex/normal (keine Texturkoordinaten)
            size_t pos = vertex.find('/');
            std::string vertexPart = vertex.substr(0, pos);
            std::string normalPart = vertex.substr(pos + 1);
            
            try {
                int vertexIndex = std::stoi(vertexPart); // Vertex Index
                int normalIndex = std::stoi(normalPart); // Normal Index

                obj.vertexIndices.push_back(vertexIndex);
                obj.uvIndices.push_back(0);  // Kein Texturindex
                obj.normalIndices.push_back(normalIndex);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ungültiger Index: " << vertexPart << " oder " << normalPart << std::endl;
                return;
            } catch (const std::out_of_range& e) {
                std::cerr << "Index außerhalb des gültigen Bereichs: " << vertexPart << " oder " << normalPart << std::endl;
                return;
            }
        }
        else if (slashCount == 2) {
            // Format: vertex/texture/normal (mit Texturkoordinaten)
            size_t pos1 = vertex.find('/');
            size_t pos2 = vertex.find('/', pos1 + 1);
            
            std::string vertexPart = vertex.substr(0, pos1);
            std::string texturePart = vertex.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string normalPart = vertex.substr(pos2 + 1);

            try {
                int vertexIndex = std::stoi(vertexPart); // Vertex Index
                int textureIndex = (texturePart.empty()) ? 0 : std::stoi(texturePart); // Textur Index (leer = 0)
                int normalIndex = std::stoi(normalPart); // Normal Index

                obj.vertexIndices.push_back(vertexIndex);
                obj.uvIndices.push_back(textureIndex);
                obj.normalIndices.push_back(normalIndex);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ungültiger Index: " << vertexPart << ", " << texturePart << " oder " << normalPart << std::endl;
                return;
            } catch (const std::out_of_range& e) {
                std::cerr << "Index außerhalb des gültigen Bereichs: " << vertexPart << ", " << texturePart << " oder " << normalPart << std::endl;
                return;
            }
        } else {
            std::cerr << "Ungültiges Face-Format: " << vertex << std::endl;
        }
    }

    // Materialindex setzen
    if (obj.currentMaterial) {
        obj.materialIndices.push_back(obj.currentMaterialIndex);
    } else {
        std::cerr << "-- name: KEIN CURRENTMAT VORHANDEN" << std::endl;
        obj.materialIndices.push_back(-1); // Kein Material vorhanden
    }
}

void AssetManager::parseMtlFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der MTL-Datei: " << filePath << std::endl;
        return;
    }

    std::shared_ptr<Material> currentMaterial;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        // std::cout << "parseMtlFile: getline line:" << line << std::endl;
        // std::cout << "parseMtlFile: getline token:" << token << std::endl;

        if (token == "#") {
            continue;
        } else if (line.empty()) {
            continue;
        } else if (token == "newmtl") {
            std::string materialName;
            ss >> materialName;

            if(currentMaterial) {
                std::cout << "Material '" << currentMaterial->name << "' abgeschlossen, neues Material: " << materialName << std::endl;
            }

            currentMaterial = std::make_shared<Material>();
            currentMaterial->name = materialName;
            currentMaterial->ambient = {0.0f, 0.0f, 0.0f, 1.0f};
            currentMaterial->diffuse = {0.0f, 0.0f, 0.0f, 1.0f};
            currentMaterial->specular = {0.0f, 0.0f, 0.0f, 1.0f};
            m_materials[filePath+"."+materialName] = currentMaterial;
        } else if (currentMaterial) {
            if (token == "Ka") { // Ambient
                ss >> currentMaterial->ambient[0] >> currentMaterial->ambient[1] >> currentMaterial->ambient[2];
                if (ss.fail()) { currentMaterial->ambient[3] = 0.0f; } // Falls Eingabe unvollständig
            } else if (token == "Kd") { // Diffuse
                ss >> currentMaterial->diffuse[0] >> currentMaterial->diffuse[1] >> currentMaterial->diffuse[2];
                if (ss.fail()) { currentMaterial->diffuse[3] = 1.0f; } // Standardmäßig sichtbar
            } else if (token == "Ks") { // Specular
                ss >> currentMaterial->specular[0] >> currentMaterial->specular[1] >> currentMaterial->specular[2];
                if (ss.fail()) { currentMaterial->specular[3] = 0.0f; } // Falls nicht gesetzt, kein Einfluss
            } else if (token == "d") { // Alpha (Transparenz)
                ss >> currentMaterial->diffuse[3];
            } else if (token == "Ns") { // Shininess
                ss >> currentMaterial->shininess;

                currentMaterial->shininess = std::clamp(currentMaterial->shininess, 0.0f, 128.0f);
            } else if (token == "map_Kd") { // Diffuse-Textur
                /* std::string texturePath;
                ss >> texturePath;
                std::cout << "parseMtlFile: texturePath: " << texturePath << std::endl;
                currentMaterial->textureFile = texturePath;
                currentMaterial->textureID = loadTexture(texturePath, "/rd/" + texturePath); // Lade Textur und speichere die ID */
            } else {
                std::cerr << "Unbekanntes Token in MTL-Datei: " << token << std::endl;
            }
        } else {
            std::cerr << "Token '" << token << "' gefunden, aber kein aktives Material definiert." << std::endl;
        }
    }

    if (currentMaterial) {
        std::cout << "Material '" << currentMaterial->name << "' abgeschlossen." << std::endl;
        m_materials[filePath + "." + currentMaterial->name] = currentMaterial; // Letztes Material speichern!
    }
}

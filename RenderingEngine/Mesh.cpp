#include <fstream>
#include <sstream>
#include "Mesh.h"
#include "../Logger.h"

namespace GUESS::rendering::threed {

    Mesh::Mesh() {
    }

    bool Mesh::loadFromOBJ(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to open OBJ file: " + filepath);
            return false;
        }

        std::vector<GUESS::core::math::Vector3f> tempVertices;
        std::vector<GUESS::core::math::Vector2f> tempUVs;
        std::vector<GUESS::core::math::Vector3f> tempNormals;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                tempVertices.push_back(GUESS::core::math::Vector3f(x, y, z));
            }
            else if (type == "vt") {
                float u, v;
                iss >> u >> v;
                tempUVs.push_back(GUESS::core::math::Vector2f(u, v));
            }
            else if (type == "vn") {
                float x, y, z;
                iss >> x >> y >> z;
                tempNormals.push_back(GUESS::core::math::Vector3f(x, y, z));
            }
            else if (type == "f") {
                for (int i = 0; i < 3; i++) {
                    std::string vertexData;
                    iss >> vertexData;

                    std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                    std::istringstream vertexStream(vertexData);

                    unsigned int vertexIndex, uvIndex, normalIndex;
                    vertexStream >> vertexIndex >> uvIndex >> normalIndex;

                    indices.push_back(vertexIndex - 1);

                    Vertex3d vertex;
                    if (vertexIndex <= tempVertices.size()) {
                        vertex.position = tempVertices[vertexIndex - 1];
                    }
                    if (uvIndex <= tempUVs.size()) {
                        vertex.texCoords = GUESS::core::math::Vector2f(tempUVs[uvIndex - 1].x, tempUVs[uvIndex - 1].y);
                    }
                    if (normalIndex <= tempNormals.size()) {
                        vertex.normal = tempNormals[normalIndex - 1];
                        vertex.color = sf::Color(
                            static_cast<sf::Uint8>((vertex.normal.x + 1.0f) * 127.5f),
                            static_cast<sf::Uint8>((vertex.normal.y + 1.0f) * 127.5f),
                            static_cast<sf::Uint8>((vertex.normal.z + 1.0f) * 127.5f)
                        );
                    }
                    vertices.push_back(vertex);
                }
            }
        }

        return true;
    }

    bool Mesh::loadFromFBX(const std::string& filepath) {
        GUESS::core::Logger::log(GUESS::core::Logger::WARNING, "FBX loading not yet implemented");
        return false;
    }

    bool Mesh::loadFromBlend(const std::string& filepath) {
        GUESS::core::Logger::log(GUESS::core::Logger::WARNING, "Blend file loading not yet implemented");
        return false;
    }

    void Mesh::setVertices(const std::vector<Vertex3d>& newVertices) {
        vertices = newVertices;
    }

    void Mesh::setIndices(const std::vector<unsigned int>& newIndices) {
        indices = newIndices;
    }

    void Mesh::setPosition(const GUESS::core::math::Vector3f& position) {
        m_position = position;
        updateTransform();
    }

    void Mesh::setRotation(const GUESS::core::math::Vector3f& rotation) {
        m_rotation = rotation;
        updateTransform();
    }

    void Mesh::setScale(const GUESS::core::math::Vector3f& scale) {
        m_scale = scale;
        updateTransform();
    }

    void Mesh::updateTransform() {
        m_transform = GUESS::core::math::Matrix4x4::translation(m_position.x, m_position.y, m_position.z) *
            GUESS::core::math::Matrix4x4::rotation(m_rotation.x, 1, 0, 0) *
            GUESS::core::math::Matrix4x4::rotation(m_rotation.y, 0, 1, 0) *
            GUESS::core::math::Matrix4x4::rotation(m_rotation.z, 0, 0, 1) *
            GUESS::core::math::Matrix4x4::scale(m_scale.x, m_scale.y, m_scale.z);
    }

    void Mesh::calculateBoundingBox() {
        if (vertices.empty()) {
            m_boundingBox = GUESS::core::math::AABB();
            return;
        }

        GUESS::core::math::Vector3f min = vertices[0].position;
        GUESS::core::math::Vector3f max = vertices[0].position;

        for (const auto& vertex : vertices) {
            min.x = std::min(min.x, vertex.position.x);
            min.y = std::min(min.y, vertex.position.y);
            min.z = std::min(min.z, vertex.position.z);

            max.x = std::max(max.x, vertex.position.x);
            max.y = std::max(max.y, vertex.position.y);
            max.z = std::max(max.z, vertex.position.z);
        }

        m_boundingBox = GUESS::core::math::AABB(min, max);
    }
 
}

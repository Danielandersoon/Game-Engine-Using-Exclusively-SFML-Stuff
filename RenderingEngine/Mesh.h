#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "Vertex3d.h"

namespace GUESS::rendering::threed {
    class Mesh {
    private:
        std::vector<Vertex3d> vertices;
        std::vector<unsigned int> indices;

        GUESS::core::math::Matrix4x4 m_transform;
        GUESS::core::math::Vector3f m_position;
        GUESS::core::math::Vector3f m_rotation;
        GUESS::core::math::Vector3f m_scale;

        GUESS::core::math::AABB m_boundingBox;
        void calculateBoundingBox();

    public:
        Mesh();
        bool loadFromOBJ(const std::string& filepath);
        bool loadFromFBX(const std::string& filepath);
        bool loadFromBlend(const std::string& filepath);

        const std::vector<Vertex3d>& getVertices() const { return vertices; }
        const std::vector<unsigned int>& getIndices() const { return indices; }

        void setVertices(const std::vector<Vertex3d>& newVertices);
        void setIndices(const std::vector<unsigned int>& newIndices);

        void setPosition(const GUESS::core::math::Vector3f& position);
        void setRotation(const GUESS::core::math::Vector3f& rotation);
        void setScale(const GUESS::core::math::Vector3f& scale);

        const GUESS::core::math::Matrix4x4& getTransform() const { return m_transform; }
        void updateTransform();

        const GUESS::core::math::AABB& getBoundingBox() const { return m_boundingBox; }
    };
}
#endif

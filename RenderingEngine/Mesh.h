#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "./Vertex3d.h"

namespace GUESS::rendering::threed {
    class Mesh {
    private:
        std::vector<Vertex3d> vertices;
        std::vector<unsigned int> indices;
        mutable std::vector<GUESS::core::math::Matrix4x4> instanceTransforms;

        GUESS::core::math::Matrix4x4 m_transform;
        GUESS::core::math::Vector3f m_position;
        GUESS::core::math::Vector3f m_rotation;
        GUESS::core::math::Vector3f m_scale;

        GUESS::core::math::AABB m_boundingBox;

        std::unique_ptr<sf::VertexBuffer> vertexBuffer;
        std::unique_ptr<sf::VertexBuffer> indexBuffer;

        void calculateBoundingBox();

    public:
        Mesh();
        Mesh(Mesh&& other) noexcept = default;
        Mesh& operator=(Mesh&& other) noexcept = default;

        ~Mesh() {
            cleanup();
        }

        void cleanup() {
            vertices.clear();
            indices.clear();
            vertexBuffer.reset();
            indexBuffer.reset();
        }
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
        void setInstanceTransforms(const std::vector<GUESS::core::math::Matrix4x4>& transforms) const { instanceTransforms = transforms;  };

        Mesh* get() { return this; }
        const GUESS::core::math::Matrix4x4& getTransform() const { return m_transform; }
        void updateTransform();

        const GUESS::core::math::Vector3f& getPosition() const { return m_position; }
        const sf::VertexBuffer* getVertexArray() const { return vertexBuffer.get(); }
        const GUESS::core::math::AABB& getBoundingBox() const { return m_boundingBox; }

        void addInstance(const GUESS::core::math::Matrix4x4& transform) {
            instanceTransforms.push_back(transform);
        }

        const std::vector<GUESS::core::math::Matrix4x4>& getInstanceTransforms() const {
            return instanceTransforms;
        }

    };
}
#endif

#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H
#include "../Component.h"
#include "Mesh.h"
#include "Material.h"

namespace GUESS::rendering::threed {
    class MeshRendererComponenet : public GUESS::core::Component {
        GUESS::rendering::threed::Mesh* mesh;
        GUESS::rendering::Material* material;
    public:
        GUESS::rendering::threed::Mesh* getMesh() const { return mesh; }
        GUESS::rendering::Material* getMaterial() const { return material; }
    };
}

#endif
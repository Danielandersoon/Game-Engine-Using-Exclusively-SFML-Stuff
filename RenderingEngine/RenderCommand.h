#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "../GMath.h"

namespace GUESS::rendering {
    struct RenderCommand {
        const GUESS::rendering::threed::Mesh* mesh;
        Material* material;
        GUESS::core::math::Matrix4x4 transform;
    };
}
#endif
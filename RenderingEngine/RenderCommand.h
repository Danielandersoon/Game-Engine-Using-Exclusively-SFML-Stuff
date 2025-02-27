#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H
#include "./Camera.h"
#include "./Mesh.h"
#include "./Material.h"
#include "../GMath.h"

namespace GUESS::rendering::twod {
    class Sprite;  // Forward declaration
}

namespace GUESS::rendering {
    struct RenderCommand {
        union {
            const GUESS::rendering::threed::Mesh* mesh;
            const GUESS::rendering::twod::Sprite* sprite;
        } geometry;
        const GUESS::rendering::Material* material;
        bool is2D;
        bool useInstancing = false;
    };
}
#endif

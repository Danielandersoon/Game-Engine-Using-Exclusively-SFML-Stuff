#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H
#include <map>
#include <vector>
#include "RenderCommand.h"
#include "Camera.h"

namespace GUESS::rendering {
    class RenderQueue {
    private:
        std::map<int, std::vector<RenderCommand>> queues;
        struct LODLevel {
            float distance;
            std::shared_ptr<GUESS::rendering::threed::Mesh> mesh;
        };

        std::map<GUESS::rendering::threed::Mesh*, std::vector<LODLevel>> lodLevels;


    public:
        void submit(const RenderCommand& cmd, int priority);
        void execute(const Camera& camera, sf::RenderTarget& renderTarget); 
        void clear() {}

        void addLODLevel(GUESS::rendering::threed::Mesh* baseMesh, float distance, std::shared_ptr<GUESS::rendering::threed::Mesh> lodMesh);
        GUESS::rendering::threed::Mesh* selectLOD(GUESS::rendering::threed::Mesh* mesh, float distance);

    };
}
#endif
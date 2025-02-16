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

    public:
        void submit(const RenderCommand& cmd, int priority);
        void execute(const Camera& camera, sf::RenderTarget& renderTarget); 
        void clear() {}
    };
}
#endif
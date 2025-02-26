#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <unordered_map>
#include <memory>
#include "PhysicsMaterial.h"

namespace GUESS::physics {
    class PhysicsManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<PhysicsMaterial>> materials;

    public:
        // Material management
        void registerMaterial(const std::string& name, std::unique_ptr<PhysicsMaterial> material);
        PhysicsMaterial* getMaterial(const std::string& name);
        void removeMaterial(const std::string& name);

        float getCombinedFriction(const std::string& matA, const std::string& matB);
        float getCombinedRestitution(const std::string& matA, const std::string& matB);

        // Preset materials
        void initializeDefaultMaterials();
    };
}

#endif

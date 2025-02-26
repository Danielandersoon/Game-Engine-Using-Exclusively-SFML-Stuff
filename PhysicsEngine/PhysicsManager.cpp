#include "PhysicsManager.h"
#include "../Logger.h"

namespace GUESS::physics {
    void PhysicsManager::registerMaterial(const std::string& name, std::unique_ptr<PhysicsMaterial> material) {
        if (materials.find(name) == materials.end()) {
            materials[name] = std::move(material);
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "Registered physics material: " + name);
        }
    }

    PhysicsMaterial* PhysicsManager::getMaterial(const std::string& name) {
        auto it = materials.find(name);
        if (it != materials.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void PhysicsManager::removeMaterial(const std::string& name) {
        materials.erase(name);
    }

    void PhysicsManager::initializeDefaultMaterials() {
        // Ice material
        registerMaterial("Ice", std::make_unique<PhysicsMaterial>(
            "Ice", 0.02f, 0.01f, 0.4f, 0.9f
        ));

        // Metal material
        registerMaterial("Metal", std::make_unique<PhysicsMaterial>(
            "Metal", 0.6f, 0.4f, 0.8f, 7.8f
        ));

        // Rubber material
        registerMaterial("Rubber", std::make_unique<PhysicsMaterial>(
            "Rubber", 0.9f, 0.7f, 0.95f, 1.2f
        ));

        // Wood material
        registerMaterial("Wood", std::make_unique<PhysicsMaterial>(
            "Wood", 0.5f, 0.3f, 0.3f, 0.7f
        ));

        registerMaterial("Bouncy", std::make_unique < PhysicsMaterial>(
            "Bouncy", 0.1f, 0.1f, 0.95f, 0.3f));

        registerMaterial("Sticky", std::make_unique < PhysicsMaterial>(
            "Sticky", 0.95f, 0.94f, 0.1f, 1.2f));
    }


    float PhysicsManager::getCombinedFriction(const std::string& matA, const std::string& matB)
    {
        auto* materialA = getMaterial(matA);
        auto* materialB = getMaterial(matB);

        if (materialA && materialB) {
            return PhysicsMaterial::combineFriction(*materialA, *materialB);
        }
        return 0.5f; // Default friction if materials not found
    }

    float PhysicsManager::getCombinedRestitution(const std::string& matA, const std::string& matB) {
        auto* materialA = getMaterial(matA);
        auto* materialB = getMaterial(matB);

        if (materialA && materialB) {
            return PhysicsMaterial::combineRestitution(*materialA, *materialB);
        }
        return 0.5f; // Default restitution if materials not found
    }

}

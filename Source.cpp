#include "./InstanceManager.h"
#include "./GameObject.h"
#include "./MeshRendererComponent.h"
#include "./LightComponents.h"
#include "./CameraComponent.h"
#include <iostream>
#include "./RigidbodyComponent.h"
#include "./PhysicsEngine/BoxColliderComponent.h"

int main() {
    try {
        GUESS::core::InstanceManager instanceManager;

        auto* engine = instanceManager.getInstanceByID(1);
        if (!engine) {
            std::cerr << "Engine instance not found" << std::endl;
            return -1;
        }

        auto* sceneManager = engine->getSceneManager();
        auto& scene = sceneManager->getCurrentScene();

		// Cube 1 - Center, falling from high
		auto cubeObject = std::make_unique<GUESS::core::GameObject>("Cube1");
		cubeObject->getTransform().setPosition(GUESS::core::math::Vector3f(0.0f, 50.0f, -65.0f));
		cubeObject->getTransform().setScale(GUESS::core::math::Vector3f(10.0f, 10.0f, 10.0f));

		auto* meshRenderer = cubeObject->addComponent<GUESS::core::MeshRendererComponent>();
		meshRenderer->loadMesh("cube.obj");

		auto* rbComp = cubeObject->addComponent<GUESS::core::RigidbodyComponent>();
		rbComp->init();
		rbComp->setMass(5.0f);
		rbComp->setRestitution(0.3f);
		auto* colComp = cubeObject->addComponent<GUESS::core::BoxColliderComponent>(GUESS::core::math::Vector3f(20.0f, 20.0f, 20.0f));

		// Cube 2 - Left position, higher
		auto cubeObjectTwo = std::make_unique<GUESS::core::GameObject>("Cube2");
		cubeObjectTwo->getTransform().setPosition(GUESS::core::math::Vector3f(-50.0f, 80.0f, -65.0f));
		cubeObjectTwo->getTransform().setScale(GUESS::core::math::Vector3f(10.0f, 10.0f, 10.0f));

		auto* meshRendererTwo = cubeObjectTwo->addComponent<GUESS::core::MeshRendererComponent>();
		meshRendererTwo->loadMesh("cube.obj");

		auto* rbCompTwo = cubeObjectTwo->addComponent<GUESS::core::RigidbodyComponent>();
		rbCompTwo->init();
		rbCompTwo->setMass(2.0f);
		rbCompTwo->setRestitution(0.3f);
		auto* colCompTwo = cubeObjectTwo->addComponent<GUESS::core::BoxColliderComponent>(GUESS::core::math::Vector3f(20.0f, 20.0f, 20.0f));

		// Cube 3 - Right position, highest, WITH ROTATION to test physics
		auto cubeObjectThree = std::make_unique<GUESS::core::GameObject>("Cube3");
		cubeObjectThree->getTransform().setPosition(GUESS::core::math::Vector3f(50.0f, 110.0f, -65.0f));
		cubeObjectThree->getTransform().setScale(GUESS::core::math::Vector3f(10.0f, 10.0f, 10.0f));
		// Apply initial rotation - cube should tumble as it falls
		cubeObjectThree->getTransform().setRotation(GUESS::core::math::Vector3f(0.0f, 45.0f * 3.14159f / 180.0f, 30.0f * 3.14159f / 180.0f));

		auto* meshRendererThree = cubeObjectThree->addComponent<GUESS::core::MeshRendererComponent>();
		meshRendererThree->loadMesh("cube.obj");

		auto* rbCompThree = cubeObjectThree->addComponent<GUESS::core::RigidbodyComponent>();
		rbCompThree->init();
		rbCompThree->setMass(3.0f);
		rbCompThree->setRestitution(0.4f);
		auto* colCompThree = cubeObjectThree->addComponent<GUESS::core::BoxColliderComponent>(GUESS::core::math::Vector3f(20.0f, 20.0f, 20.0f));

		// Cube 4 - Far left, with different rotation
		auto cubeObjectFour = std::make_unique<GUESS::core::GameObject>("Cube4");
		cubeObjectFour->getTransform().setPosition(GUESS::core::math::Vector3f(-100.0f, 140.0f, -65.0f));
		cubeObjectFour->getTransform().setScale(GUESS::core::math::Vector3f(10.0f, 10.0f, 10.0f));
		// Tilted rotation
		cubeObjectFour->getTransform().setRotation(GUESS::core::math::Vector3f(60.0f * 3.14159f / 180.0f, 0.0f, 20.0f * 3.14159f / 180.0f));

		auto* meshRendererFour = cubeObjectFour->addComponent<GUESS::core::MeshRendererComponent>();
		meshRendererFour->loadMesh("cube.obj");

		auto* rbCompFour = cubeObjectFour->addComponent<GUESS::core::RigidbodyComponent>();
		rbCompFour->init();
		rbCompFour->setMass(4.0f);
		rbCompFour->setRestitution(0.2f);
		auto* colCompFour = cubeObjectFour->addComponent<GUESS::core::BoxColliderComponent>(GUESS::core::math::Vector3f(20.0f, 20.0f, 20.0f));

        // Create a static ground box for the cubes to land on
        auto ground = std::make_unique<GUESS::core::GameObject>("Ground");
        ground->getTransform().setPosition(GUESS::core::math::Vector3f(0.0f, -75.0f, -65.0f));
        ground->getTransform().setScale(GUESS::core::math::Vector3f(200.0f, 10.0f, 200.0f));

        auto* groundRenderer = ground->addComponent<GUESS::core::MeshRendererComponent>();
        // You can use a cube mesh stretched to be the ground
        groundRenderer->loadMesh("cube.obj");

        // Add collider and a heavy rigidbody to act as static
        auto* groundRb = ground->addComponent<GUESS::core::RigidbodyComponent>();
        groundRb->init();
        groundRb->setMass(1e8f); // effectively immovable
        groundRb->setFriction(0.8f);
        groundRb->setRestitution(0.0f);
        // cube.obj size is 2.0, so collider must be: 2.0 * scale = (400, 20, 400)
        auto* groundCol = ground->addComponent<GUESS::core::BoxColliderComponent>(GUESS::core::math::Vector3f(400.0f, 20.0f, 400.0f), true);

        auto lightObject = std::make_unique<GUESS::core::GameObject>("MainLight");
        lightObject->getTransform().setPosition(GUESS::core::math::Vector3f(100.0f, 200.0f, -165.0f));
        auto* lightComponent = lightObject->addComponent<GUESS::core::LightComponent>();
        lightComponent->type = GUESS::rendering::threed::LightType::Point;
        lightComponent->colour = GUESS::core::math::Vector3f(240.0f, 240.0f, 220.0f);
        lightComponent->intensity = 700.2f;
        lightComponent->range = 1800.0f;

        // Create camera object with angled view (10 degrees down)
        auto cameraObject = std::make_unique<GUESS::core::GameObject>("MainCamera");
        cameraObject->getTransform().setPosition(GUESS::core::math::Vector3f(0.0f, 0.0f, 50.0f));
        // Rotate 10 degrees around X axis (pitch down)
        cameraObject->getTransform().setRotation(GUESS::core::math::Vector3f(-10.0f * 3.14159f / 180.0f, -10.0f, 0.0f));
        auto* cameraComp = cameraObject->addComponent<GUESS::core::CameraComponent>();
        cameraComp->isMainCamera = true;

        scene.AddGameObject(std::move(cubeObject));
        scene.AddGameObject(std::move(cubeObjectTwo));
        scene.AddGameObject(std::move(cubeObjectThree));
        scene.AddGameObject(std::move(cubeObjectFour));
        scene.AddGameObject(std::move(ground));
        scene.AddGameObject(std::move(lightObject));
        scene.AddGameObject(std::move(cameraObject));
        engine->start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing GUESS engine: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
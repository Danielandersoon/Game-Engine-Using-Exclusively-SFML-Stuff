#include <fstream>
#include "./Logger.h"
#include "./SceneSerializer.h"
#include "./BoxCollisionComponent.h"
#include "./MeshRendererComponent.h"
#include "./Rigidbody2DComponent.h"
#include "./RigidbodyComponent.h"
#include "./SpriteRenderComponent.h"
#include "./CameraComponent.h"

namespace GUESS::core {
    bool SceneSerializer::saveScene(Scene& scene, const std::string& filepath) {
        JsonValue root(std::map<std::string, JsonValue>{});
        root.set("sceneName", JsonValue(scene.GetSceneName()));

        // Serialize camera
        auto& camera = scene.getMainCamera();
        std::vector<JsonValue> cameraPos;
        cameraPos.push_back(JsonValue(static_cast<double>(camera.getPosition().x)));
        cameraPos.push_back(JsonValue(static_cast<double>(camera.getPosition().y)));
        cameraPos.push_back(JsonValue(static_cast<double>(camera.getPosition().z)));

        JsonValue cameraData(std::map<std::string, JsonValue>{});
        cameraData.set("position", JsonValue(cameraPos));
        root.set("camera", cameraData);

        // Serialize game objects
        std::vector<JsonValue> gameObjectsArray;
        for (const auto& gameObject : scene.GetGameObjects()) {
            gameObjectsArray.push_back(serializeGameObject(gameObject.second.get()));
        }
        root.set("gameObjects", JsonValue(gameObjectsArray));

        // Write to file
        std::string jsonStr = JsonParser::stringify(root, true);
        std::ofstream file(filepath);
        if (!file.is_open()) {
            Logger::log(Logger::ERROR, "Failed to open file for scene serialization: " + filepath);
            return false;
        }

        file << jsonStr;
        return true;
    }

    bool SceneSerializer::loadScene(Scene& scene, const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            Logger::log(Logger::ERROR, "Failed to open scene file: " + filepath);
            return false;
        }

        std::string jsonContent((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        JsonValue root = JsonParser::parse(jsonContent);
        if (root.isNull()) {
            Logger::log(Logger::ERROR, "Failed to parse scene file");
            return false;
        }

        scene.CloseScene();

        // Load camera settings
        const auto& camPos = root.get<std::map<std::string, JsonValue>>()["camera"]
            .get<std::map<std::string, JsonValue>>()["position"]
            .get<std::vector<JsonValue>>();

        scene.getMainCamera().setPosition(GUESS::core::math::Vector3f(
            static_cast<float>(camPos[0].get<double>()),
            static_cast<float>(camPos[1].get<double>()),
            static_cast<float>(camPos[2].get<double>())
        ));

        // Load game objects
        const auto& gameObjects = root.get<std::map<std::string, JsonValue>>()["gameObjects"]
            .get<std::vector<JsonValue>>();
        for (const auto& objValue : gameObjects) {
            scene.AddGameObject(deserializeGameObject(objValue));
        }

        scene.LoadScene();
        return true;
    }


    JsonValue SceneSerializer::serializeGameObject(const GameObject* gameObject) {
        JsonValue objValue(std::map<std::string, JsonValue>{});

        // Serialize basic properties
        objValue.set("name", JsonValue(gameObject->getName()));
        objValue.set("active", JsonValue(gameObject->isActive()));

        // Serialize transform
        const Transform& transform = gameObject->getTransform(); // Get as const reference
        JsonValue transformValue(std::map<std::string, JsonValue>{});

        // Create temporary Vector3f values to store the transform data
        GUESS::core::math::Vector3f position = transform.getPosition();
        GUESS::core::math::Vector3f rotation = transform.getRotation().toEuler(); // Assuming you have a method to get Euler angles
        GUESS::core::math::Vector3f scale = transform.getScale();

        transformValue.set("position", serializeVector3(position));
        transformValue.set("rotation", serializeVector3(rotation));
        transformValue.set("scale", serializeVector3(scale));
        objValue.set("transform", transformValue);

        // Serialize components
        JsonValue components = serializeComponents(gameObject);
        objValue.set("components", components);

        return objValue;
    }

    JsonValue SceneSerializer::serializeComponents(const GameObject* gameObject) {
        JsonValue components(std::vector<JsonValue>{});

        // Serialize MeshRenderer component
        if (auto* meshRenderer = gameObject->getComponent<MeshRendererComponent>()) {
            JsonValue meshRendererData(std::map<std::string, JsonValue>{});
            meshRendererData.set("type", JsonValue("MeshRenderer"));
            meshRendererData.set("meshPath", JsonValue(meshRenderer->meshPath));
            meshRendererData.set("castShadows", JsonValue(meshRenderer->castShadows));
            meshRendererData.set("receiveShadows", JsonValue(meshRenderer->receiveShadows));
            components.append(meshRendererData);
        }

        // Serialize SpriteRenderer component
        if (auto* spriteRenderer = gameObject->getComponent<SpriteRendererComponent>()) {
            JsonValue spriteRendererData(std::map<std::string, JsonValue>{});
            spriteRendererData.set("type", JsonValue("SpriteRenderer"));
            spriteRendererData.set("spritePath", JsonValue(spriteRenderer->spritePath));
            spriteRendererData.set("flipX", JsonValue(spriteRenderer->flipX));
            spriteRendererData.set("flipY", JsonValue(spriteRenderer->flipY));
            components.append(spriteRendererData);
        }

        // Serialize Camera component
        if (auto* camera = gameObject->getComponent<CameraComponent>()) {
            JsonValue cameraData(std::map<std::string, JsonValue>{});
            cameraData.set("type", JsonValue("Camera"));
            cameraData.set("fieldOfView", JsonValue(static_cast<double>(camera->fieldOfView)));
            cameraData.set("nearPlane", JsonValue(static_cast<double>(camera->nearPlane)));
            cameraData.set("farPlane", JsonValue(static_cast<double>(camera->farPlane)));
            cameraData.set("isMainCamera", JsonValue(camera->isMainCamera));
            components.append(cameraData);
        }

        // Serialize Rigidbody components
        if (auto* rb3d = gameObject->getComponent<RigidbodyComponent>()) {
            JsonValue rbData(std::map<std::string, JsonValue>{});
            rbData.set("type", JsonValue("Rigidbody3D"));
            components.append(rbData);
        }

        if (auto* rb2d = gameObject->getComponent<Rigidbody2DComponent>()) {
            JsonValue rbData(std::map<std::string, JsonValue>{});
            rbData.set("type", JsonValue("Rigidbody2D"));
            components.append(rbData);
        }

        return components;
    }

    void SceneSerializer::deserializeComponents(GameObject* gameObject, const JsonValue& jsonObj) {
        const auto& components = jsonObj.get<std::vector<JsonValue>>();

        for (const auto& componentData : components) {
            const auto& data = componentData.get<std::map<std::string, JsonValue>>();
            std::string type = data.at("type").get<std::string>();

            if (type == "MeshRenderer") {
                auto* meshRenderer = gameObject->addComponent<MeshRendererComponent>();
                meshRenderer->meshPath = data.at("meshPath").get<std::string>();
                meshRenderer->castShadows = data.at("castShadows").get<bool>();
                meshRenderer->receiveShadows = data.at("receiveShadows").get<bool>();
                meshRenderer->loadMesh(meshRenderer->meshPath);
            }
            else if (type == "SpriteRenderer") {
                auto* spriteRenderer = gameObject->addComponent<SpriteRendererComponent>();
                spriteRenderer->spritePath = data.at("spritePath").get<std::string>();
                spriteRenderer->flipX = data.at("flipX").get<bool>();
                spriteRenderer->flipY = data.at("flipY").get<bool>();
                spriteRenderer->loadSprite(spriteRenderer->spritePath);
            }
            else if (type == "Camera") {
                auto* camera = gameObject->addComponent<CameraComponent>();
                camera->fieldOfView = static_cast<float>(data.at("fieldOfView").get<double>());
                camera->nearPlane = static_cast<float>(data.at("nearPlane").get<double>());
                camera->farPlane = static_cast<float>(data.at("farPlane").get<double>());
                camera->isMainCamera = data.at("isMainCamera").get<bool>();
            }
            else if (type == "Rigidbody3D") {
                gameObject->addComponent<RigidbodyComponent>();
            }
            else if (type == "Rigidbody2D") {
                gameObject->addComponent<Rigidbody2DComponent>();
            }
        }
    }

    std::unique_ptr<GameObject> SceneSerializer::deserializeGameObject(const JsonValue& jsonObj) {
        const auto& data = jsonObj.get<std::map<std::string, JsonValue>>();

        // Create new game object
        auto gameObject = std::make_unique<GameObject>();

        // Set basic properties
        gameObject->setName(data.at("name").get<std::string>());
        gameObject->setActive(data.at("active").get<bool>());

        // Deserialize transform
        const auto& transformData = data.at("transform").get<std::map<std::string, JsonValue>>();
        auto position = deserializeVector3(transformData.at("position"));
        auto rotation = deserializeVector3(transformData.at("rotation"));
        auto scale = deserializeVector3(transformData.at("scale"));

        gameObject->getTransform().setPosition(position);
        gameObject->getTransform().setRotation(GUESS::core::math::Quaternion::fromEuler(rotation.x, rotation.y, rotation.z));
        gameObject->getTransform().setScale(scale);

        // Deserialize components
        if (data.find("components") != data.end()) {
            deserializeComponents(gameObject.get(), data.at("components"));
        }

        return gameObject;
    }

}

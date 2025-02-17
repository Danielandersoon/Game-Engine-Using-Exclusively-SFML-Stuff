#include <fstream>
#include "Logger.h"
#include "SceneSerializer.h"

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
            gameObjectsArray.push_back(serializeGameObject(gameObject.get()));
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

    std::unique_ptr<GameObject> SceneSerializer::deserializeGameObject(const JsonValue& jsonObj) {
        const auto& objData = jsonObj.get<std::map<std::string, JsonValue>>();

        // Create game object with name
        auto gameObject = std::make_unique<GameObject>(
            objData.at("name").get<std::string>()
        );

        // Set active state
        gameObject->setActive(objData.at("active").get<bool>());

        // Deserialize transform
        const auto& transformData = objData.at("transform").get<std::map<std::string, JsonValue>>();
        GUESS::core::math::Vector3f position = deserializeVector3(transformData.at("position"));
        GUESS::core::math::Vector3f rotation = deserializeVector3(transformData.at("rotation"));
        GUESS::core::math::Vector3f scale = deserializeVector3(transformData.at("scale"));

        // Set the transform values
        gameObject->getTransform().setPosition(position);
        gameObject->getTransform().setRotation(rotation);
        gameObject->getTransform().setScale(scale);

        // Deserialize components
        deserializeComponents(gameObject.get(), objData.at("components"));

        return gameObject;
    }
    JsonValue SceneSerializer::serializeComponents(const GameObject* gameObject) {
        JsonValue components(std::vector<JsonValue>{});
        // Add component serialization logic here
        return components;
    }

    void SceneSerializer::deserializeComponents(GameObject* gameObject, const JsonValue& jsonObj) {
        // Add component deserialization logic here
    }
}

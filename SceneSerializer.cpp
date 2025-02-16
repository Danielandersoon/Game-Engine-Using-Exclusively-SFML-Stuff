#include <fstream>
#include "Logger.h"
#include "SceneSerializer.h"
#include "JsonParser.h"

namespace GUESS::core {
    bool SceneSerializer::saveScene(const Scene& scene, const std::string& filepath) {
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
}

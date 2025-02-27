#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "./Scene.h"
#include "./JsonParser.h"

namespace GUESS::core {
    class SceneSerializer {
    private:
        static JsonValue serializeVector3(const GUESS::core::math::Vector3f& vec) {
            std::vector<JsonValue> values;
            values.push_back(JsonValue(static_cast<double>(vec.x)));
            values.push_back(JsonValue(static_cast<double>(vec.y)));
            values.push_back(JsonValue(static_cast<double>(vec.z)));
            return JsonValue(values);
        }

        static GUESS::core::math::Vector3f deserializeVector3(const JsonValue& json) {
            const auto& values = json.get<std::vector<JsonValue>>();
            return GUESS::core::math::Vector3f(
                static_cast<float>(values[0].get<double>()),
                static_cast<float>(values[1].get<double>()),
                static_cast<float>(values[2].get<double>())
            );
        }


    public:
        static bool saveScene(Scene& scene, const std::string& filepath);
        static bool loadScene(Scene& scene, const std::string& filepath);

    private:
        static JsonValue serializeGameObject(const GameObject* gameObject);
        static std::unique_ptr<GameObject> deserializeGameObject(const JsonValue& jsonObj);
        static JsonValue serializeTransform(const Transform& transform);
        static void deserializeTransform(Transform& transform, const JsonValue& jsonObj);
        static JsonValue serializeComponents(const GameObject* gameObject);
        static void deserializeComponents(GameObject* gameObject, const JsonValue& jsonObj);
    };
}
#endif

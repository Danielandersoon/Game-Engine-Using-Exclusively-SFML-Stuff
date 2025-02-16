#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "Scene.h"
#include "JsonValue.h"

namespace GUESS::core {
    class SceneSerializer {
    public:
        static bool saveScene(const Scene& scene, const std::string& filepath);
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

#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H
#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include <memory>
#include <vector>

namespace GUESS::core {
    class GameObject;

    struct Event {
        std::string name;
        GameObject* sender;
        std::any data;
    };

    class EventSystem {
    private:
        using EventCallback = std::function<void(const Event&)>;
        std::unordered_map<std::string, std::vector<std::pair<GameObject*, EventCallback>>> subscribers;

    public:
        void subscribe(const std::string& eventName, GameObject* receiver, EventCallback callback) {
            subscribers[eventName].push_back({ receiver, callback });
        }

        void unsubscribe(const std::string& eventName, GameObject* receiver) {
            auto& callbacks = subscribers[eventName];
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [receiver](const auto& pair) { return pair.first == receiver; }),
                callbacks.end()
            );
        }

        void publish(const std::string& eventName, GameObject* sender, const std::any& data = std::any()) {
            Event event{ eventName, sender, data };
            for (const auto& [receiver, callback] : subscribers[eventName]) {
                callback(event);
            }
        }

        void clearAllSubscriptions() {
            subscribers.clear();
        }
    };
}
#endif

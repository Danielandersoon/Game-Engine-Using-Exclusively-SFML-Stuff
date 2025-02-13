#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H
#include <unordered_map>
#include <string>
#include <functional>

namespace GUESS::core {
    class EventSystem {
    private:
        std::unordered_map<std::string, std::vector<std::function<void(const void*)>>> subscribers;
        
    public:
        template<typename T>
        void subscribe(const std::string& eventName, std::function<void(const T&)> callback) {
            subscribers[eventName].push_back([callback](const void* data) {
                callback(*static_cast<const T*>(data));
            });
        }
        
        template<typename T>
        void publish(const std::string& eventName, const T& data) {
            for(auto& callback : subscribers[eventName]) {
                callback(&data);
            }
        }
    };
}

#endif
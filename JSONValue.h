#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <string>
#include <vector>
#include <map>
#include <variant>

namespace GUESS::core {
    class JsonValue {
    public:
        enum Type {
            Null,
            Number,
            String,
            Boolean,
            Array,
            Object
        };

        using ValueType = std::variant<
            std::nullptr_t,
            double,
            std::string,
            bool,
            std::vector<JsonValue>,
            std::map<std::string, JsonValue>
        >;

        JsonValue() : type(Null), value(nullptr) {}
        JsonValue(double num) : type(Number), value(num) {}
        JsonValue(const std::string& str) : type(String), value(str) {}
        JsonValue(bool b) : type(Boolean), value(b) {}
        JsonValue(const std::vector<JsonValue>& arr) : type(Array), value(arr) {}
        JsonValue(const std::map<std::string, JsonValue>& obj) : type(Object), value(obj) {}

        Type getType() const { return type; }

        // Comparison operators for container operations
        bool operator==(const JsonValue& other) const {
            return type == other.type && value == other.value;
        }

        bool operator!=(const JsonValue& other) const {
            return !(*this == other);
        }

        // Type checking methods
        bool isNull() const { return type == Null; }
        bool isNumber() const { return type == Number; }
        bool isString() const { return type == String; }
        bool isBoolean() const { return type == Boolean; }
        bool isArray() const { return type == Array; }
        bool isObject() const { return type == Object; }

        template<typename T>
        T get() const { return std::get<T>(value); }

        void append(const JsonValue& val);
        void set(const std::string& key, const JsonValue& val);

        std::string toString() const;
        static JsonValue parse(const std::string& json);

    private:
        Type type;
        ValueType value;
    };
}

#endif

#include "JsonValue.h"
#include "JsonParser.h"

namespace GUESS::core {

    void JsonValue::append(const JsonValue& val) {
        if (type != Array) {
            type = Array;
            value = std::vector<JsonValue>();
        }
        std::get<std::vector<JsonValue>>(value).push_back(val);
    }

    void JsonValue::set(const std::string& key, const JsonValue& val) {
        if (type != Object) {
            type = Object;
            value = std::map<std::string, JsonValue>();
        }
        std::get<std::map<std::string, JsonValue>>(value)[key] = val;
    }

    std::string JsonValue::toString() const {
        return JsonParser::stringify(*this);
    }

    JsonValue JsonValue::parse(const std::string& json) {
        return JsonParser::parse(json);
    }
}

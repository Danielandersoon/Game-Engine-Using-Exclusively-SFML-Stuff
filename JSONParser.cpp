#include "JsonParser.h"
#include <sstream>
#include <iomanip>

namespace GUESS::core {

    const char* JsonParser::current = nullptr;
    const char* JsonParser::end = nullptr;

    JsonValue JsonParser::parse(const std::string& input) {
        current = input.c_str();
        end = current + input.length();

        skipWhitespace();
        return parseValue();
    }

    std::string JsonParser::stringify(const JsonValue& value, bool pretty) {
        std::stringstream ss;

        switch (value.getType()) {
        case JsonValue::Null:
            ss << "null";
            break;

        case JsonValue::Number:
            ss << value.get<double>();
            break;

        case JsonValue::String:
            ss << '"' << value.get<std::string>() << '"';
            break;

        case JsonValue::Boolean:
            ss << (value.get<bool>() ? "true" : "false");
            break;

        case JsonValue::Array: {
            ss << "[";
            const auto& array = value.get<std::vector<JsonValue>>();
            for (size_t i = 0; i < array.size(); ++i) {
                if (i > 0) ss << ",";
                if (pretty) ss << "\n  ";
                ss << stringify(array[i], pretty);
            }
            if (pretty) ss << "\n";
            ss << "]";
            break;
        }

        case JsonValue::Object: {
            ss << "{";
            const auto& obj = value.get<std::map<std::string, JsonValue>>();
            bool first = true;
            for (const auto& [key, val] : obj) {
                if (!first) ss << ",";
                if (pretty) ss << "\n  ";
                first = false;
                ss << '"' << key << "\":" << stringify(val, pretty);
            }
            if (pretty) ss << "\n";
            ss << "}";
            break;
        }
        }

        return ss.str();
    }
}

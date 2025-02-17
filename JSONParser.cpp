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

    void JsonParser::skipWhitespace() {
        while (current < end && std::isspace(*current)) {
            current++;
        }
    }

    JsonValue JsonParser::parseValue() {
        skipWhitespace();

        if (*current == '{') {
            return parseObject();
        }
        else if (*current == '[') {
            return parseArray();
        }
        else if (*current == '"') {
            return JsonValue(parseString());
        }
        else if (std::isdigit(*current) || *current == '-') {
            return JsonValue(parseNumber());
        }
        else if (strncmp(current, "true", 4) == 0) {
            current += 4;
            return JsonValue(true);
        }
        else if (strncmp(current, "false", 5) == 0) {
            current += 5;
            return JsonValue(false);
        }
        else if (strncmp(current, "null", 4) == 0) {
            current += 4;
            return JsonValue();
        }

        return JsonValue();
    }
    JsonValue JsonParser::parseObject() {
        // Implementation for parsing JSON objects
        JsonValue obj(std::map<std::string, JsonValue>{});
        match('{');
        skipWhitespace();

        while (current < end && *current != '}') {
            std::string key = parseString();
            skipWhitespace();
            match(':');
            JsonValue value = parseValue();
            obj.set(key, value);

            skipWhitespace();
            if (*current == ',') {
                current++;
                skipWhitespace();
            }
        }
        match('}');
        return obj;
    }

    JsonValue JsonParser::parseArray() {
        std::vector<JsonValue> array;
        match('[');
        skipWhitespace();

        while (current < end && *current != ']') {
            array.push_back(parseValue());
            skipWhitespace();
            if (*current == ',') {
                current++;
                skipWhitespace();
            }
        }
        match(']');
        return JsonValue(array);
    }

    std::string JsonParser::parseString() {
        std::string str;
        match('"');
        while (current < end && *current != '"') {
            str += *current++;
        }
        match('"');
        return str;
    }

    double JsonParser::parseNumber() {
        std::string numStr;
        while (current < end && (std::isdigit(*current) || *current == '-' || *current == '.')) {
            numStr += *current++;
        }
        return std::stod(numStr);
    }
    bool JsonParser::match(char expected) {
        if (current < end && *current == expected) {
            current++;
            skipWhitespace();
            return true;
        }
        return false;
    }
}

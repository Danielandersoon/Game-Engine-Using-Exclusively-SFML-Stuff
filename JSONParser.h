#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "JsonValue.h"
#include <string>

namespace GUESS::core {
    class JsonParser {
    public:
        static JsonValue parse(const std::string& input);
        static std::string stringify(const JsonValue& value, bool pretty = false);

    private:
        static JsonValue parseValue();
        static JsonValue parseObject();
        static JsonValue parseArray();
        static std::string parseString();
        static double parseNumber();

        static void skipWhitespace();
        static bool match(char expected);

        static const char* current;
        static const char* end;
    };
}

#endif

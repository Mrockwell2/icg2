#include "Value/StringValue.hpp"
#include <iostream>
#include <sstream>

StringValue::StringValue(const char* value) {
    this->value = value;
};

StringValue::StringValue(std::string value) {
    this->value = value;
};

void StringValue::print(std::ostream &s) const {
    s << value;
}

std::string StringValue::toString() const {
    return getRawString();
}

std::string StringValue::getRawString() const {
    return value;
}

std::string StringValue::getEscapedString() const {
    std::stringstream ss;

    ss << "\"";

    for (int i = 0 ; i < value.size() ; i++) {
        if (isprint(value[i])) {
            ss << value[i];
        } else {
            switch (value[i]) {
                case '\n': ss << "\\n"; break;
                case '\t': ss << "\\t"; break;
                case '\b': ss << "\\b"; break;
                case '\a': ss << "\\a"; break;
                case '\f': ss << "\\f"; break;
                case '\r': ss << "\\n"; break;
                case '\v': ss << "\\v"; break;
                case '\"': ss << "\\\""; break;
                default  : {
                    char temp_s[6];
                    sprintf(temp_s, "\\x%02x", value[i]);
                    ss << temp_s ; 
                    break;
                }
            }
        }
    }
    ss << "\"";
    return ss.str();
}

#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"
#include <sstream>

// CONSTRUCTOR
IntegerValue::IntegerValue(long long value) {
    this->value = value;
};

// MEMBER FUNCTION
void IntegerValue::print(std::ostream &s) const {
    s << value;
}

// MEMBER FUNCTION
std::string IntegerValue::toString() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// MEMBER FUNCTION
double IntegerValue::getFloatingPointValue() const {
    return (double)value;
}

// MEMBER FUNCTION
long long IntegerValue::getIntegerValue() const {
    return (long long)value;
}


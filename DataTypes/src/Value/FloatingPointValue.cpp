#include "Value/FloatingPointValue.hpp"
#include "Value/IntegerValue.hpp"
#include <sstream>

// CONSTRUCTOR
FloatingPointValue::FloatingPointValue(double value) {
    this->value = value;
};

// MEMBER FUNCTION
void FloatingPointValue::print(std::ostream &s) const {
    s << value;
}

// MEMBER FUNCTION
std::string FloatingPointValue::toString() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// MEMBER FUNCTION
double FloatingPointValue::getFloatingPointValue() const {
    return (double)value;
}

// MEMBER FUNCTION
long long FloatingPointValue::getIntegerValue() const {
    return (long long)value;
}


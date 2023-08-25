#include "Value/PointerValue.hpp"
#include <sstream>

// CONSTRUCTOR
PointerValue::PointerValue(void * value) {
    this->value = value;
}

// MEMBER FUNCTION
void* PointerValue::getPointer() {
    return value;
}

// MEMBER FUNCTION
void  PointerValue::print(std::ostream &s) const {
    s << value;
}

// MEMBER FUNCTION
std::string PointerValue::toString() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

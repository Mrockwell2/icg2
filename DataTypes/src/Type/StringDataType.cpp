#include <iostream>
#include <sstream>

#include "Type/StringDataType.hpp"
#include "Value/StringValue.hpp"


StringDataType::StringDataType() {}

/* ==================================================================== */
/*                           VIRTUAL INTERFACE                          */
/* ==================================================================== */

// DataType * StringDataType::clone () const {
//     return new StringDataType( *this );
// }

bool StringDataType::validate(DataTypeInator * dataTypeInator) {
    return true;
}

bool StringDataType::isValid() const {
    return true;
}

size_t StringDataType::getSize() const {
    return sizeof (std::string);
}

void* StringDataType::createInstance(unsigned int num) const {
    void* temp = (void*)calloc(num, sizeof(std::string));
    return temp;
}

void StringDataType::deleteInstance(void* address) const {
    free(address);
}

// MEMBER FUNCTION
std::string StringDataType::getTypeSpecName() const {
    return "std::string";
}

bool StringDataType::accept (DataTypeVisitor* visitor) const {
    return visitor->visitStringType(std::static_pointer_cast<const StringDataType>(shared_from_this()));
}


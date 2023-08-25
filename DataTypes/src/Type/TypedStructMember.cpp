#include <iostream>

#include "Type/TypedStructMember.hpp"


// Real constructor
TypedStructMember::TypedStructMember(std::string name,  DataTypeInator* dataTypeInator, std::string typeSpecName) 
                        : StructMember(name) , typeSpecName(typeSpecName),  subType(NULL), dataTypeInator(dataTypeInator), isValid(false){}

TypedStructMember::TypedStructMember (const TypedStructMember & original) : StructMember(original) {
    typeSpecName = original.typeSpecName;
    subType = original.subType;
    dataTypeInator = original.dataTypeInator;
    isValid = original.isValid;
}

bool TypedStructMember::validate() {

    if (!isValid) {
        subType = dataTypeInator->resolve( typeSpecName );

        if (subType != NULL) {
            isValid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName
                        << "\" not found in the DataTypeInator." << std::endl;
        }
    }
    return isValid;
}

// MEMBER FUNCTION
std::shared_ptr<const DataType> TypedStructMember::getDataType() const {
    return subType;
}

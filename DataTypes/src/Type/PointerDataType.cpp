#include <iostream>
#include <sstream>
#include <stdlib.h> // for free()

#include "Type/PointerDataType.hpp"
#include "Value/PointerValue.hpp"
#include "Utils/MutableDeclaration.hpp"

#include "DataTypeInator.hpp"




// CONSTRUCTOR
PointerDataType::PointerDataType( std::string typeSpecifierName) : typeSpecName(typeSpecifierName) {}

// COPY CONSTRUCTOR
// PointerDataType::PointerDataType ( PointerDataType const & original) {

//     is_valid = original.is_valid;
//     typeSpecName = original.typeSpecName;

//     subType = original.subType;
  
// }

// DESTRUCTOR
PointerDataType::~PointerDataType () {}

// CLONE
// DataType * PointerDataType::clone () const {
//     return new PointerDataType( *this );
// }

// MEMBER FUNCTION
bool PointerDataType::validate(DataTypeInator * dataTypeInator) {

    if (!is_valid) {
        subType = dataTypeInator->resolve( typeSpecName );
        if (subType != NULL) {
            is_valid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" not found." << std::endl;
        }
    }
    return is_valid;
}

bool PointerDataType::isValid() const {
    return is_valid;
}

// MEMBER FUNCTION
size_t PointerDataType::getSize() const {
    return sizeof(void*);
}

// MEMBER FUNCTION
void* PointerDataType::createInstance(unsigned int num) const {
    void* temp = calloc(num, sizeof(void*));
    return temp;
}

// MEMBER FUNCTION
void PointerDataType::deleteInstance(void* address) const {
    free(address);
}


// MEMBER FUNCTION
std::string PointerDataType::getTypeSpecName() const {
    return typeSpecName;
}

// MEMBER FUNCTION
std::string PointerDataType::makeDeclaration(std::string declarator) const {
    // Parse the typename as a declaration
    MutableDeclaration decl(getTypeSpecName());
    
    // Put the current dimension on the front
    decl.pushDimension(-1);

    return decl.getDeclarator(declarator);
}

bool PointerDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitPointerType(std::static_pointer_cast<const PointerDataType>(shared_from_this()));
}

std::shared_ptr<const DataType> PointerDataType::getSubType() const {
    return subType;
}
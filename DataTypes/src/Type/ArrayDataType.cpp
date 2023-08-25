#include <iostream>
#include <sstream>
#include <cassert>

#include "Type/ArrayDataType.hpp"

#include "Utils/MutableDeclaration.hpp"

// CONSTRUCTOR
ArrayDataType:: ArrayDataType( std::string typeSpecName, unsigned int n_elems ) : 
                        typeSize(0), elementCount(n_elems), is_valid(false), typeSpecName(typeSpecName) {
    subType = NULL;

    if (elementCount == 0) {
        throw std::logic_error("An ArrayDataType cannot have 0 elements.");
    }
}


/* ================================================================================= */
/*                         RULE OF THREE (and a half) INTERFACE                      */
/* ================================================================================= */
// ArrayDataType::ArrayDataType ( ArrayDataType const & original) {
//     is_valid = original.is_valid;
//     typeSpecName = original.typeSpecName;
//     elementCount = original.elementCount;
//     subType = original.subType;
// }



// DESTRUCTOR
ArrayDataType::~ArrayDataType () {

}


// MEMBER FUNCTION
bool ArrayDataType::validate(DataTypeInator * dataTypeInator) {

    if (!is_valid) {

        subType = dataTypeInator->resolve( typeSpecName );

        if (subType == NULL) {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" not found." << std::endl;
        } else {
            is_valid = true;
            typeSize = elementCount * subType->getSize();
        }
        
    }
    return is_valid;
}

bool ArrayDataType::isValid() const {
    return is_valid;
}

// MEMBER FUNCTION
size_t ArrayDataType::getSize() const {

    if (is_valid) {
        return typeSize;
    } else {
        std::cerr << "ERROR: Can't getSize() because type hasn't been validated." << std::endl;
        return 0;
    }
}

// MEMBER FUNCTION
unsigned int ArrayDataType::getTotalElementCount() const {
    return elementCount;
}


// MEMBER FUNCTION
void* ArrayDataType::createInstance(unsigned int num) const {

    if (subType != NULL) {
        return subType->createInstance( num * elementCount );
    } else {
        std::cerr << "ERROR: Can't create an instance of an unvalidated type." << std::endl;
        return (void*) NULL;
    }
}

// MEMBER FUNCTION
void ArrayDataType::deleteInstance(void* address) const {
    if (subType != NULL) {
        return subType->deleteInstance(address);
    } else {
        std::cout << "WARNING: Can't delete an instance of an unvalidated type." << std::endl;
    }
}


// MEMBER FUNCTION
std::string ArrayDataType::getTypeSpecName() const {
    return typeSpecName;
}

// MEMBER FUNCTION
std::string ArrayDataType::makeDeclaration(std::string declarator) const {

    // Parse the typename as a declaration
    MutableDeclaration decl(getTypeSpecName());

    // Push this dimension
    decl.pushDimension(elementCount);

    // Appropriately attach this dimension
    return decl.getDeclarator(declarator);
}

// MEMBER FUNCTION
unsigned int ArrayDataType::getElementCount() const {
    return elementCount;
}

bool ArrayDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitArrayType(std::static_pointer_cast<const ArrayDataType>(shared_from_this()));
}

std::shared_ptr<const DataType> ArrayDataType::getSubType() const {
    return subType;
}


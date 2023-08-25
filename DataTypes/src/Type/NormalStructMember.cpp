#include <sstream>
#include <iostream>

#include "Type/NormalStructMember.hpp"
#include "DataTypeInator.hpp"


// CONSTRUCTOR
NormalStructMember::NormalStructMember(std::string memberName, int offset, std::string typeSpecName ) 
                                        : StructMember(memberName, typeSpecName), byte_offset(offset) {}


// NormalStructMember * NormalStructMember::clone () const {
//     return new NormalStructMember( *this );
// }

// MEMBER FUNCTION
int NormalStructMember::getOffset() const {
    return byte_offset;
}

void * NormalStructMember::getAddress(void * struct_address) const {
    return (char*)struct_address + byte_offset;;
}

std::string NormalStructMember:: toString() const {
    if (isValid()) {
        return getSubType()->makeDeclaration(getName());
    } else {
        return "<unvalidated type>";
    }
}

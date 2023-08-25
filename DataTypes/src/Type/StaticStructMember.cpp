#include <sstream>
#include <iostream>

#include "Type/StaticStructMember.hpp"
#include "DataTypeInator.hpp"



// CONSTRUCTOR
StaticStructMember::StaticStructMember(std::string memberName,void * memberAddress,std::string typeSpecName) 
                                    :  StructMember(memberName, typeSpecName), memberAddress(memberAddress) {}


// StaticStructMember * StaticStructMember::clone () const {
//     return new StaticStructMember( *this );
// }

void * StaticStructMember::getAddress() const {
    return memberAddress;
}

// MEMBER FUNCTION
std::string StaticStructMember::toString() const {
    std::ostringstream oss;
    oss << "static ";
    if (isValid()) {
        oss << getSubType()->makeDeclaration(getName());
    } else {
        oss << "<unvalidated type>";
    }
    return oss.str();
}

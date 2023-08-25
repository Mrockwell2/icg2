#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "Type/StaticStructMember.hpp"

#include <iostream>
#include <sstream>

CompositeDataType::CompositeDataType() {

    this->is_valid = false;
    this->structSize = 0;
}

// CONSTRUCTOR
CompositeDataType::CompositeDataType( std::string name,
                                      size_t structSize,
                                      void* (*allocator)(int),
                                      void (*deAllocator)(void*) ) {
    this->is_valid = false;
    this->name = name;
    this->structSize = structSize;
    this->allocator = allocator;
    this->deAllocator = deAllocator;
}



// DESTRUCTOR
CompositeDataType::~CompositeDataType() {
    for (auto member : normalMemberList) {
        delete member;
    }

    for (auto member : staticMemberList) {
        delete member;
    }
}


bool CompositeDataType::validate(DataTypeInator * dataTypeInator) {

    // (from John) FIXME: Check for circular dependencies.
    // Also need a to check that this->subType (ptr) is not equal
    // to any of the  member->subType's. Otherwise we will have a circular reference.

    // Notes from Jackie: Is the circular dependency something that we really want to address?
    // This is not meant to be a compiler, and a circular dependency would not compile. 
    // I think allowing for undefined behavior if the user is trying to represent types that cannot be represented in C++ is ok

    if (!is_valid) {

        is_valid = true;

        for (auto member : normalMemberList) {
            is_valid &= member->validate(dataTypeInator);
        }

        for (auto member : staticMemberList) {
            is_valid &= member->validate(dataTypeInator);
        }

    }
    return is_valid;
}

bool CompositeDataType::isValid () const {
    return is_valid;
}

// MEMBER FUNCTION
size_t CompositeDataType::getSize() const {
    return structSize;
}

// MEMBER FUNCTION
void* CompositeDataType::createInstance(unsigned int num) const {

    void* address;
    if (allocator == NULL) {
        // Allocate using calloc.
        if ((address = calloc( (size_t)num, (size_t)structSize ))  == NULL) {
            std::cerr << "ERROR: Out of memory." << std::endl;
            return ((void*)NULL);
        }
    } else {
        // Allocate using the allocator.
        if ((address = allocator( num)) == NULL) {
            std::cerr << "ERROR: Out of memory." << std::endl;
            return ((void*)NULL);
        }
    }
    return address;
}

// MEMBER FUNCTION
void CompositeDataType::deleteInstance(void* address) const {

    if (allocator != NULL) {
        if(deAllocator != NULL) {
            deAllocator(address);
        } else {
            std::cerr << "Warning: CompositeDataType " << getTypeSpecName() << " was initialized with an allocator, but not a deallocator." << std::endl;
        }
    } else {
        free(address);
    }
}

// MEMBER FUNCTION
std::string CompositeDataType::toString() const {
   std::stringstream ss;
   ss << "composite {\n";
   for (auto member : normalMemberList) {
       ss << member->toString();
       ss << ";\n";
   }

   for (auto member : staticMemberList) {
       ss << member->toString();
       ss << ";\n";
   }
   ss << "}\n";
   return ss.str();
}

// MEMBER FUNCTION
std::string CompositeDataType::getTypeSpecName() const {
    return name;
}

bool CompositeDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitCompositeType(std::static_pointer_cast<const CompositeDataType>(shared_from_this()));
}


// MEMBER FUNCTION
void CompositeDataType::addRegularMember( std::string memberName,
                                               int member_offset,
                                               std::string typeSpecName )  {

    if (hasMemberNamed(memberName)) {
        throw std::logic_error( "ERROR: Attempt to re-define member \"" + memberName + "\"." );
    }

    normalMemberList.push_back(new NormalStructMember( memberName, member_offset, typeSpecName ));
}

// MEMBER FUNCTION
void CompositeDataType::addStaticMember( std::string memberName,
                                         void * memberAddress,
                                         std::string typeSpecName )  {

    if (hasMemberNamed(memberName)) {
        throw std::logic_error( "ERROR: Attempt to re-define member \"" + memberName + "\"." );
    }

    staticMemberList.push_back(new StaticStructMember( memberName, memberAddress, typeSpecName ));
}

int CompositeDataType::getNormalMemberCount() const {
    return normalMemberList.size() ;
}

int CompositeDataType::getStaticMemberCount() const {
    return staticMemberList.size() ;
}


bool CompositeDataType::hasMemberNamed(std::string name) {
    for (auto member : normalMemberList) {
        if (name == member->getName()) return true;
    }

    for (auto member : staticMemberList) {
        if (name == member->getName()) return true;
    }

    return false;
}
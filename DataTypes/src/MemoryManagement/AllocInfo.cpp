#include <sstream>
#include <string.h> // for memcpy

#include "Type/VisitableTypes.hpp"
#include "MemoryManagement/AllocInfo.hpp"
#include "Utils/MutableDeclaration.hpp"
#include "Algorithm/DataTypeAlgorithm.hpp"



const std::string AllocInfo::localPrefix = "trick_local_";
const std::string AllocInfo::externPrefix = "trick_extern_";
unsigned int AllocInfo::nextSerialNumber = 0;

// PRIVATE MEMBER FUNCTION
void AllocInfo::initialize( const std::string& varName,
                            std::shared_ptr<const DataType>    type,
                            void*              suppliedAllocation)  {

    // Because they can be resized, all allocations are really arrays,
    // whether they have 0, 1, or many elements.
    // uhhhhhh no -jackie
    
    name = varName;
    dataType = type;

    if ( suppliedAllocation != 0 ) {
        start = suppliedAllocation;
        storageClass = StorageClass::EXTERN;
    } else {
        start = dataType->createInstance(1);
        storageClass = StorageClass::LOCAL;
    }

    serialNumber = nextSerialNumber++;

    end = (char*)start + dataType->getSize();
}


// CONSTRUCTOR
AllocInfo::AllocInfo( const std::string& varName,
                      std::shared_ptr<const DataType>    type,
                      void*              suppliedAllocation)  {

    initialize(varName, type, suppliedAllocation);
}

AllocInfo::~AllocInfo() {
    // Only destroy the underlying allocation if it is was allocated by us.
    if (storageClass == StorageClass::LOCAL) {
        dataType->deleteInstance(start);
    }
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getName() const {
    return name;
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getSerialName() const {
    std::ostringstream oss;
    if (storageClass == StorageClass::LOCAL) {
        oss << localPrefix << serialNumber ;
    } else {
        oss << externPrefix << serialNumber ;
    }
    return oss.str();
}

// PUBLIC MEMBER FUNCTION
unsigned int AllocInfo::getSerialNumber() const {
    return serialNumber;
}

// PUBLIC MEMBER FUNCTION
void* AllocInfo::getStart() const {
    return start;
}

// PUBLIC MEMBER FUNCTION
int AllocInfo::getSize() const {
    return dataType->getSize();
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getTypeSpecifierName() const {
    return dataType->getTypeSpecName();
}

// PUBLIC MEMBER FUNCTION
std::shared_ptr<const DataType> AllocInfo::getDataType() const {
    return dataType;
}

// PUBLIC MEMBER FUNCTION
StorageClass::e AllocInfo::getStorageClass() const {
    return storageClass;
}

// PUBLIC MEMBER FUNCTION
void AllocInfo::clear() const {
    DataTypeAlgorithm::clearValue(dataType, start);
}



// PUBLIC MEMBER FUNCTION
bool AllocInfo::contains(void* address) const {

    if (( address >= start) && (address <= end)) {
        return true;
    }
    return false;
}


// PUBLIC MEMBER FUNCTION
std::string AllocInfo::toString() const {

    std::ostringstream oss;
    if (storageClass == StorageClass::LOCAL) {
        oss << "LOCAL ";
    } else {
        oss << "EXTERN ";
    }
    oss << "<" << getDataType()->makeDeclaration( name) << ">";
    oss << " @";
    oss << std::hex << start << ".." ;
    oss << std::hex << end   << " " ;
    oss << "\tValue: ";
    // getDataType()->printValue(oss, start);
    oss << std::endl;
    return oss.str();
}


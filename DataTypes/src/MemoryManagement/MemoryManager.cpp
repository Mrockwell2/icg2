#include <iostream>
#include <fstream>
#include <algorithm>

#include "MemoryManagement/MemoryManager.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "Algorithm/DataTypeAlgorithm.hpp"

#include "Utils/MutableDeclaration.hpp"

#include "CheckpointAgent/J_CheckpointAgent.hpp"
#include "DataTypeInator.hpp"



// Constructor
MemoryManager::MemoryManager (DataTypeInator * dictionary) : dataTypeInator(dictionary) {
   debugLevel = 0;

   checkpointAgent = new J_CheckpointAgent(dataTypeInator);
   pthread_mutex_init(&allocInfoMapMutex, NULL);
}

MemoryManager::~MemoryManager () {
    delete checkpointAgent;
   
   // Delete all the AllocInfos
    for (auto it : allocInfoByAddressMap) {
        delete it.second;
    }

    // Both maps have the same thing, just indexed in different ways, so we only need to delete out of one of them.
}

void* MemoryManager::do_declare_var(const std::string& abstract_declarator, 
                             const std::string& variable_name,
                             void * supplied_allocation) {

    // Don't redeclare
    if ( var_exists( variable_name )) {
        std::cerr << "ERROR: Variable " << variable_name << " already declared." << std::endl;
        return ((void*)NULL);
    }

    // Look up the type
    std::shared_ptr<const DataType> type = dataTypeInator->resolve(abstract_declarator);
    if (type == NULL) {
        std::cerr << "ERROR: Unable to resolve type string \"" << abstract_declarator << "\" for variable \"" << variable_name << "\", cannot declare." << std::endl;
        return ((void*)NULL);
    }

    // Make the AllocInfo
    AllocInfo* newAllocInfo = new AllocInfo(variable_name, type, supplied_allocation);

    // Set the address and insert in the maps
    void * actualAllocation = newAllocInfo->getStart();

    allocInfoByAddressMap[actualAllocation] = newAllocInfo;
    allocInfoByNameMap[variable_name] = newAllocInfo;

    return( actualAllocation );
}

// MEMBER FUNCTION
void* MemoryManager::declare_var( const std::string& declaration,
                           void*              suppliedAllocation ) {

    MutableDeclaration typeName(declaration);
    std::string abstractDeclarator = typeName.getAbstractDeclarator();
    std::string variableName = typeName.getVariableName();

    return do_declare_var( abstractDeclarator, variableName, suppliedAllocation );
}

// MEMBER FUNCTION
bool MemoryManager::var_exists( const std::string& variableName) {

    if ( !variableName.empty()) {
        AllocInfo* allocInfo = getAllocInfoNamed( variableName );
        if ( allocInfo != (AllocInfo*)NULL ) {
            return true;
        }
    }
    return false;
}

// MEMBER FUNCTION
void MemoryManager::clear_var( void* address) {

    AllocInfo* allocInfo;
    if ((allocInfo = getAllocInfoOf( address)) != NULL) {
        allocInfo->clear();
    } else {
        std::cerr << "Cannot clear the variable at address " << address << " because memory manager knows nothing about it." << std::endl;
    }
}

// MEMBER FUNCTION
void MemoryManager::clear_var( const std::string& variableName) {

    AllocInfo* allocInfo = getAllocInfoNamed( variableName );
    if (allocInfo != NULL) {
        allocInfo->clear();
    } else {
        std::cerr << "Cannot clear the variable named " << variableName << " because memory manager knows nothing about it." << std::endl;
    }
}

// MEMBER FUNCTION
void MemoryManager::clear_all_vars() {
    for (auto it : allocInfoByAddressMap) {
        it.second->clear();
    }
}

void MemoryManager::delete_var(void* address) {
    AllocInfo* allocInfo = getAllocInfoAt( address);

    if (allocInfo != NULL) {
        delete_allocation(allocInfo);
    } else {
        std::cerr << "Cannot delete the variable at address " << address << " because memory manager knows nothing about it." << std::endl;
    }
}

void MemoryManager::delete_var(std::string var_name) {
    AllocInfo* allocInfo = getAllocInfoNamed( var_name);

    if (allocInfo != NULL) {
        delete_allocation(allocInfo);
    } else {
        std::cerr << "Cannot delete the variable named " << var_name << " because memory manager knows nothing about it." << std::endl;
    }
}

void MemoryManager::delete_allocation(AllocInfo * allocInfo) {
    pthread_mutex_lock(&allocInfoMapMutex);
    allocInfoByAddressMap.erase(allocInfo->getStart());
    allocInfoByNameMap.erase(allocInfo->getName());
    pthread_mutex_unlock(&allocInfoMapMutex);

    // This will properly handle the extern vs local deletion
    delete allocInfo;
}


// MEMBER FUNCTION
void MemoryManager::write_checkpoint( const std::string& filename) {

    std::ofstream out_s( filename.c_str(), std::ios::out);
    if (out_s.is_open()) {
        write_checkpoint( out_s );
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

// MEMBER FUNCTION
void MemoryManager::write_checkpoint( std::ostream& out_s) {

    AllocInfo* allocInfo;
    std::vector<AllocInfo*> allocInfoList;

    pthread_mutex_lock(&allocInfoMapMutex);

    for ( auto pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        allocInfo = pos->second;
        allocInfoList.push_back(allocInfo);
    }
    do_write_checkpoint( out_s, allocInfoList);

    pthread_mutex_unlock(&allocInfoMapMutex);
}

// MEMBER FUNCTION
void MemoryManager::write_partial_checkpoint( const std::string& filename, const std::string& var_name) {

    std::ofstream out_s( filename.c_str(), std::ios::out);
    if (out_s.is_open()) {
        write_partial_checkpoint( out_s, var_name );
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

// MEMBER FUNCTION
void MemoryManager::write_partial_checkpoint( std::ostream& out_s, const std::string& var_name) {

    AllocInfo* allocInfo;
    std::vector<AllocInfo*> allocInfoList;
	FindDependencies::Result result;

    pthread_mutex_lock(&allocInfoMapMutex);

	allocInfo = getAllocInfoNamed(var_name);
	if(allocInfo) {
		result = DataTypeAlgorithm::findDependencies(allocInfo->getDataType(), allocInfo->getStart());
	} else {
		std::cerr << "ERROR: Couldn't find \"" << var_name << "\" allocated." << std::endl;
	}

    for ( auto addr = result.begin() ; addr != result.end() ; addr++ ) {
        allocInfo = getAllocInfoOf(*addr);
		if(allocInfo) {
        	allocInfoList.push_back(allocInfo);
		} else {
			std::cerr << "ERROR: Dependency of \"" << var_name << "\" at \"" << (char*)(*addr) << "\" is not allocated." << std::endl;
		}
    }
    do_write_checkpoint( out_s, allocInfoList);

    pthread_mutex_unlock(&allocInfoMapMutex);
}


// PRIVATE MEMBER FUNCTION
void MemoryManager::do_write_checkpoint(std::ostream& outStream, std::vector<AllocInfo*>& allocInfoList) {
    checkpointAgent->dump(outStream, allocInfoList);
}

void MemoryManager::restore_checkpoint(const std::string& filename) {
    std::ifstream in_s( filename.c_str(), std::ios::in);
    if (in_s.is_open()) {
        restore_checkpoint( in_s) ;
    } else {
        std::cerr << "ERROR: Couldn't open \""<< filename <<"\"." << std::endl;
        std::cerr.flush();
    }
}

void MemoryManager::restore_checkpoint( std::istream& in_s) {
    // TODO: We should be holding a lock here
    
    // Only pass external allocations into the agent
    std::vector<AllocInfo *> extern_alloc_list;
    for ( auto pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        AllocInfo * allocInfo = pos->second;
        if (allocInfo->getStorageClass() == StorageClass::EXTERN) {
            extern_alloc_list.push_back(allocInfo);
        }
    }

// TODO *****IMPORTANT JACKIE YOU HAVE TO THINK ABOUT THIS AT SOME POINT
// WHAT HAPPENS TO THE OLD LOCAL ALLOCS?????


    // Load the checkpoint into AllocInfos
    auto restored_alloc_list = checkpointAgent->restore(in_s, extern_alloc_list);

    // Rebuild the maps
    allocInfoByAddressMap.clear();
    allocInfoByNameMap.clear();

    for (AllocInfo * alloc : restored_alloc_list) {
        void * addr = alloc->getStart();
        std::string varname = alloc->getName();

        allocInfoByAddressMap[addr] = alloc;
        allocInfoByNameMap[varname] = alloc;
    }

    // We did it!
}


void MemoryManager::setCheckPointAgent( CheckpointAgentBase* agent) {
    delete checkpointAgent;
    checkpointAgent = agent;
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoOf( void* address ) {
    AllocInfo* allocInfo;
    std::map<void*, AllocInfo*>::iterator pos;
    for ( pos = allocInfoByAddressMap.begin() ; pos != allocInfoByAddressMap.end() ; pos++ ) {
        allocInfo = pos->second;
        if ( allocInfo->contains( address )) {
            return allocInfo;
        }
    }
    return NULL;
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoAt( void* address ) {
    std::map<void*, AllocInfo*>::iterator pos;
    pos = allocInfoByAddressMap.find( address );
    if ( pos == allocInfoByAddressMap.end()) {
        return NULL;
    } else {
        return pos->second;
    }
}

// MEMBER FUNCTION
AllocInfo* MemoryManager::getAllocInfoNamed( const std::string& name ) {
    std::map<std::string, AllocInfo*>::iterator pos;
    pos = allocInfoByNameMap.find(name);
    if ( pos == allocInfoByNameMap.end()) {
        return NULL;
    } else {
        return pos->second;
    }
}


// MEMBER FUNCTION
std::shared_ptr<const DataType> MemoryManager::getDataTypeOf( const std::string& varname ) {
    AllocInfo * allocInfo = getAllocInfoNamed(varname);
    if (allocInfo == NULL) {
        return NULL;
    }

    return allocInfo->getDataType();
}

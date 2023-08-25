#pragma once

#include <vector>
#include <map>
#include <string>
#include <pthread.h>

#include "DataTypeInator.hpp"
#include "MemoryManagement/AllocInfo.hpp"
#include "Type/DataType.hpp"

#include "CheckpointAgent/NewCheckpointAgentBase.hpp"

/**
 * @brief Keep track of allocations and their type.
 * 
 */
class MemoryManager {

    public:
    /**
     * @brief Construct a new Memory Manager 
     * 
     * @param dataTypeInator type resolver machine
     */
    MemoryManager (DataTypeInator * dataTypeInator);

    ~MemoryManager ();

    // no copying
    MemoryManager(const MemoryManager& other) = delete;
    
    /**
     * @brief Declare a new allocation of given type with the memory manager
     * 
     * @param declaration string representation of the type of this allocation
     * @param suppliedAllocation pre-existing allocation. If not given, the memory manager will allocate one
     * @return void* address of the allocation (same as suppliedAllocation if one was given). NULL if something goes wrong
     */
    void* declare_var( const std::string& declaration, void* suppliedAllocation = NULL);



    /**
     * @brief Check if a variable is registered with the memory manager
     * 
     * @param var_name variable to look for
     * @return true if var exists, false otherwise
     */
    bool var_exists( const std::string& var_name);

    /**
     * @brief Clear the value at the given address, if it is registered with the memorymanager
     * 
     * @param address address of variable to clear
     */
    void clear_var( void* address);

    /**
     * @brief Clear the variable with the given name, if it is registered with the memorymanager
     * 
     * @param var_name name of variable to clear
     */
    void clear_var( const std::string& var_name);

    /**
     * @brief Clear all variables registered with the memory manager.
     * 
     */
    void clear_all_vars();

    /**
     * @brief Delete the variable 
     * 
     * @param address address of variable to delete
     */
    void delete_var(void* address);

    /**
     * @brief Delete the variable
     * 
     * @param var_name name of variable to delete
     */
    void delete_var(std::string var_name);

    /**
     * @brief Write checkpoint to an output stream
     * 
     * @param out_s stream to write to
     */
    void write_checkpoint( std::ostream& out_s);

    /**
     * @brief Open a file and then write a checkpoint to it
     * 
     * @param filename name of file to write to
     */
    void write_checkpoint( const std::string& filename);

    /**
     * @brief Write checkpoint to an output stream
     * 
     * @param out_s stream to write to
	 * @param var_name name of the variable to be partially checkpointing
     */
    void write_partial_checkpoint( std::ostream& out_s, const std::string& var_name);

    /**
     * @brief Open a file and then write a checkpoint to it
     * 
     * @param filename name of file to write to
	 * @param var_name name of the variable to be partially checkpointing
     */
    void write_partial_checkpoint( const std::string& filename, const std::string& var_name);

    /**
     * @brief Restore a checkpoint from file with given name
     * 
     * @param filename name of file to read from
     */
    void restore_checkpoint(const std::string& filename);

    /**
     * @brief Restore a checkpoint from the given input stream
     * 
     * @param in_s stream to restore from
     */
    void restore_checkpoint( std::istream& in_s);

    /**
     * @brief Get the AllocInfo that contains the given address
     * 
     * @param address address to search for
     * @return AllocInfo* Alloc info that contains this address, or NULL if not found
     */
    AllocInfo* getAllocInfoOf( void* address );

    /**
     * @brief Get the AllocInfo that starts at the given address
     * 
     * @param address address to search for
     * @return AllocInfo* Alloc info that starts at this address, or NULL if not found
     */
    AllocInfo* getAllocInfoAt( void* address );

    /**
     * @brief Get the AllocInfo with the given name
     * 
     * @param name name to search for
     * @return AllocInfo* AllocInfo with the given name, or NULL if not found
     */
    AllocInfo* getAllocInfoNamed( const std::string& name );

    /**
     * @brief Get the DataType of the given variable
     * 
     * @param name variable name to search for
     * @return std::shared_ptr<const DataType> type of variable, or NULL if not found
     */
    std::shared_ptr<const DataType> getDataTypeOf(const std::string& name);

    /**
     * @brief Set the CheckPointAgent 
     * 
     * @param agent new CheckpointAgent object to use
     */
    void setCheckPointAgent( CheckpointAgentBase* agent);

    private:

    void do_write_checkpoint(std::ostream& outStream, std::vector<AllocInfo*>& dependencies);
    
    void* do_declare_var(const std::string& abstract_declarator, 
                                const std::string& variable_name,
                                void * supplied_allocation);

    void delete_allocation(AllocInfo * allocInfo);

    unsigned int debugLevel;

    CheckpointAgentBase * checkpointAgent;
    DataTypeInator* dataTypeInator;

    // TODO: Let's think about this.
    // I would like to switch to c++ threading instead using pthreads directly
    pthread_mutex_t allocInfoMapMutex;

    std::map<void*, AllocInfo*> allocInfoByAddressMap;
    std::map<std::string, AllocInfo*> allocInfoByNameMap;

};

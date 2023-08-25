#pragma once

#include "CheckpointAgent/NewCheckpointAgentBase.hpp"
#include "DataTypeInator.hpp"

/**
 Base Class for dumping and restoring a checkpoint.
 */
class J_CheckpointAgent : public CheckpointAgentBase {

    public:

    J_CheckpointAgent(DataTypeInator * inator);

    /**
     */
    virtual ~J_CheckpointAgent() {};


    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    /**
     * @brief Dump a checkpoint to the given string
     * 
     * @param checkpoint_out stream to output to
     * @param allocations_to_checkpoint Allocations to checkpoint
     * @return int status
     */
    virtual int dump ( std::ostream& checkpoint_out, std::vector<AllocInfo *> allocations_to_checkpoint) override;

    /**
     * @brief Restore a checkpoint from an input stream
     * 
     * @param checkpoint_in stream to read from
     * @param allocations_to_restore Extern allocs to work with
     * @return std::vector<AllocInfo *> The new set of AllocInfos (local and extern)
     */
    virtual std::vector<AllocInfo *> restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) override;


    /* ==================================================================== */
    /*                          CLASS INTERFACE                             */
    /* ==================================================================== */

    bool writeDeclaration( std::ostream& checkpoint_out, const AllocInfo * alloc);
    bool writeAssignment( std::ostream& checkpoint_out, const AllocInfo * alloc, const std::vector<AllocInfo *>& additional_search_allocs);

    AllocInfo * restoreDeclaration(std::string declaration);
    bool restoreAssignment(std::string assignment_string, const std::vector<AllocInfo *>& additional_search_allocs);


    private:

    DataTypeInator * dataTypeInator;

    static std::string getCheckpointableName(const AllocInfo * alloc);
    static std::string resolvePointer(void * ptr_to_resolve, std::shared_ptr<const DataType> expected_type, const std::vector<AllocInfo *>& allocs_to_search);
    static AllocInfo * findAllocByName(std::string name, std::vector<AllocInfo *> search_allocs);
    static void *  lookupPointer(std::string varname, const std::vector<AllocInfo *>& allocs_to_search);
    static void handleResizeCommand (std::string command_str, const std::vector<AllocInfo *>& allocs_to_search);

    static const std::string error_str ;
    static const std::string resize_command ;

};

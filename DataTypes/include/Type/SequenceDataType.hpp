#pragma once

#include "Type/DataType.hpp"
#include "DataTypeInator.hpp"

#include <stddef.h>
#include <string>
#include <stdexcept>
#include <vector>

/**
 * @brief Represents an STL sequence type - vector, list, deque, array
 * 
 */
class SequenceDataType : public DataType {

    public:

    /**
     * @brief Construct an SequenceDataType
     * 
     * @param typeSpecifierName string representation of fully qualified type
     * @param typeSize size of container
     */
    SequenceDataType( std::string typeSpecifierName, int typeSize, void *(*allocator)(int), void (*deAllocator)(void*) ) ;

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    /**
     Copy Constructor for SequenceDataType.
     */
    SequenceDataType ( const SequenceDataType & original ) = delete;

    /**
     Destructor for SequenceDataType.
     */
    ~SequenceDataType ();

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    /**
     */
     bool validate(DataTypeInator* dataTypeInator) override;

     bool isValid() const override;


    /**
     @return The size (in bytes) of an instance of the SequenceDataType.
     */
    size_t getSize() const override ;

    /**
     */
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

    /**
     */
    std::string getTypeSpecName() const override;

    /**
     */
    std::string makeDeclaration(std::string declarator) const override;

    bool accept(DataTypeVisitor * visitor) const override;



    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     * @brief Get the type of the elements of this sequence
     * 
     * @return std::shared_ptr<const DataType> 
     */
    std::shared_ptr<const DataType> getSubType() const;

    /**
     * @brief Given the address of a sequence of this type, get a list of all the addresses of the elements within.
     * 
     * @param address pointer to a sequence object
     * @return std::vector<void *> addresses of all elements
     */
    virtual std::vector<void *> getElementAddresses (void * address) const = 0;

    /**
     * @brief Given the address of a sequence of this type, get the number of elements in this sequence.
     * 
     * @param address pointer to a sequence object
     * @return int number of elements in this sequence
     */
    virtual int getNumElements (void * address) const = 0;

    /**
     * @brief resize the underlying container
     * 
     * @param address address of sequence object to resize
     * @param n_elems int num elements to resize to
     * @return bool whether the resize was successful
     */
    virtual bool resize (void * address, int n_elems) const = 0;

    /**
     * @brief clear the underlying container
     * 
     * @param address address of sequence object to clear
     * @return int num elements to resize to
     */
    virtual bool clear (void * address) const = 0;


    private:
    bool initSequenceDataType( const DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems );

    size_t typeSize;
    bool is_valid;
    std::string typeSpecName;
    std::string elemTypeName;
    std::shared_ptr<const DataType> subType;
    void* (*allocator)(int);
    void (*deAllocator)(void*);
};


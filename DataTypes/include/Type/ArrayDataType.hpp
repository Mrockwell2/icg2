#pragma once

#include "Type/DataType.hpp"
#include "DataTypeInator.hpp"

#include <stddef.h>
#include <string>
#include <stdexcept>


/**
 */
class ArrayDataType : public DataType {

    public:

    /**
     * @brief Construct an ArrayDataType
     * 
     * @param typeSpecifierName string representation of fully qualified type
     * @param n_elems Number of 
     */
    ArrayDataType( std::string typeSpecifierName,
                   unsigned int n_elems )  ;

    /**
     Create an ArrayDataType with a different elementCount, but otherwise
     identical to the original.
     */
    ArrayDataType ( const ArrayDataType & original, unsigned int newSize );

    // no default constructor allowed
    ArrayDataType() = delete;

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    /**
     Copy Constructor for ArrayDataType.
     */
    ArrayDataType ( const ArrayDataType & original ) = delete;

    /**
     Destructor for ArrayDataType.
     */
    ~ArrayDataType ();

    /**
     Assignment operator for ArrayDataType.
     @param rhs right-hand-side.
     */
    ArrayDataType& operator=( ArrayDataType rhs ) = delete;

    // friend void swap (ArrayDataType& a, ArrayDataType& b);

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    // DataType * clone () const;

    /**
     */
     bool validate(DataTypeInator* dataTypeInator) override;

     bool isValid() const override;


    /**
     @return The size (in bytes) of an instance of the ArrayDataType.
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

    std::shared_ptr<const DataType> getSubType() const;
    /**
     @return The number of arrayed Type-specifier elements of the DataType.
    */
    unsigned int getTotalElementCount() const;

    /**
     @return Returns number of elements in this level of the array
     */
    unsigned int getElementCount() const;

    private:
    bool initArrayDataType( const DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems );

    std::shared_ptr<const DataType> subType;

    size_t typeSize;
    unsigned int elementCount;
    bool is_valid;
    std::string typeSpecName;
};


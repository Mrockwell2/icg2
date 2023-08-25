#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "Type/DataType.hpp"


/**
 */
class StringDataType : public DataType {

    public:

    /**
     Constructor for ArrayDataType.
     */
    StringDataType( ) ;

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    StringDataType ( const StringDataType & original ) = delete;
    ~StringDataType () = default;
    StringDataType& operator=( const StringDataType & rhs ) = delete;

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     * @brief A StringDataType is always valid
     * 
     * @param dataTypeInator unused parameter, defaults to null
     * @return true always
     */
    bool validate(DataTypeInator * dataTypeInator = NULL) override;

    bool isValid() const override;


    /**
     @return The size (in bytes) of an instance of the PointerDataType.
     */
    size_t getSize() const override;


    /**
     Create zero or more instances of this DataType.
     */
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

    std::string getTypeSpecName () const override;

    
    bool accept (DataTypeVisitor* visitor) const override;


    private:
};

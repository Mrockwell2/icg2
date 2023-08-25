#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "DataTypeInator.hpp"


/**
 */
class PointerDataType : public DataType {

    public:

    /**
     Constructor for ArrayDataType.
     @param typeSpecifierName Name of the type on with this type is based.
     */
    PointerDataType( std::string typeSpecifierName ) ;

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    PointerDataType ( const PointerDataType & original ) = delete;
    ~PointerDataType ();
    PointerDataType & operator=( PointerDataType rhs ) = delete;


    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    bool validate(DataTypeInator * dataTypeInator) override;

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

    /**
     */
    std::string getTypeSpecName() const override;

    /**
     */
    std::string makeDeclaration(std::string declarator) const override;
    
    bool accept (DataTypeVisitor* visitor) const override;


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    std::shared_ptr<const DataType> getSubType() const;


    private:

    std::shared_ptr<const DataType> subType;

    bool             is_valid;
    std::string      typeSpecName;

    const DataTypeInator * dataTypeInator;
};

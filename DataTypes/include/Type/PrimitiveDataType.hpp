#pragma once

#include "Type/DataType.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"

#include <stdlib.h>
#include <iostream>

/**
 */
class PrimitiveDataType : public DataType {

public:

    /**
     Constructor for PrimitiveDataType.
     */
    PrimitiveDataType() {}
    ~PrimitiveDataType() {}

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    // Most of the DataTypeInterface is abstract here, will be defined by SpecifiedPrimitiveType

    bool accept (DataTypeVisitor* visitor) const override {
        return visitor->visitPrimitiveDataType(std::static_pointer_cast<const PrimitiveDataType>(shared_from_this()));
    }


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     * @brief Determine whether this is an integer type or floating point type
     * 
     * @return true this type is float or double
     * @return false otherwise
     */
    virtual bool isFloatingPoint() const = 0;
    
    /**
     * @brief Determine whether type is signed
     * @note Do we really need this?
     * 
     * @return true is signed
     * @return false type has unsigned, uint_* type 
     */
    virtual bool isSigned() const = 0;

    /**
     * @brief Is the type void?
     * 
     * @return true type is void
     * @return false type is not void
     */
    virtual bool isVoid() const = 0;

    /**
    @brief Clear the variable at the given address.
    @param address Address of the variable.
    */
    virtual void clearValue(void * address) const = 0;

    /**
    @brief Assign a value to the variable at the given address.
    @param address Address of the variable.
    @param value Value to be assigned to the element.
    */
    virtual bool assignValue(void * address, Value * value) const = 0;

    /**
    @brief Creates a Value object for the variable at the given address.
    @param address Address of the variable.
    */
    virtual Value * getValue(void *address) const = 0;

    /**
     * @brief Print an ascii representation of the value at the given address to the stream.
     */
    virtual void printValue(std::ostream &s, void *address ) const = 0;


};
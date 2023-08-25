#pragma once

#include "Type/PrimitiveDataType.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"

#include <stdlib.h>
#include <iostream>

/**
 */
template <class T>
class SpecifiedPrimitiveDataType : public PrimitiveDataType {

public:

    SpecifiedPrimitiveDataType<T> () {}

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

    SpecifiedPrimitiveDataType<T> ( const SpecifiedPrimitiveDataType<T> & original ) = default;
    ~SpecifiedPrimitiveDataType<T> () = default;
    SpecifiedPrimitiveDataType<T> & operator=( SpecifiedPrimitiveDataType<T>& rhs ) = default;

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    bool validate(DataTypeInator * dataTypeInator = NULL) override { return true; }

    bool isValid() const override { return true; }
    
    /**
     @return The size (in bytes) of an instance of the DataType
     */
    size_t getSize() const override {
        return sizeof(T);
    }

    /**
     @return an instance of the type that this PrimiitveDataType Class describes.
     */
    void* createInstance(unsigned int num) const override {
        T* temp = (T*)calloc(num, sizeof(T));
        return ((void *)temp);
    }

    /**
     */
    void deleteInstance(void* address) const override {
        delete (T*)address;
    }

    /**
     */
    void clearValue(void * address) const {
        *(T*)address =  0;
    }

    /**
     Assign a value to the variable at the given address.
     @param address Address of the variable.
     @param value Value to be assigned to the variable.
     */
    bool assignValue(void * address, Value * value) const {

        if (value->getValueType() == Value::ValueType::INTEGER || value->getValueType() == Value::ValueType::FLOATING_POINT) {
            NumericValue * numeric_value_p = static_cast<NumericValue*>(value);
            if ( isFloatingPoint() ) {
                *(T*)address =  numeric_value_p->getFloatingPointValue();
            } else {
                *(T*)address =  numeric_value_p->getIntegerValue();
            }
        } else {
            std::cerr << "ERROR: Attempt to assign non-numeric value to a numeric type." << std::endl;
            return false;
        }

        return true;
    }

    Value * getValue(void *address) const {
        if (isFloatingPoint()) {
            return new FloatingPointValue(*(T*)address);
        } else {
            return new IntegerValue(*(T*)address);
        }
    }

    /**
     Print the value of the variable at the given address, to the given stream.
     @param s The stream to print to.
     @param address Address of the variable.
     */
    void printValue(std::ostream &s, void *address ) const {
        s << *(T*)address ;
    }

    /**
     Get a string representation of this SpecifiedPrimitiveDataType's type-specifier.
     */
    std::string getTypeSpecName() const {
        std::string s("<invalid-primitive-type>");
        return s;
    }

    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     Return true if T is float or double. Otherwise return false.
     */
    bool isFloatingPoint() const { return false; }

    /**
     Return true if T is char, short, int, long, or long long. Otherwise return false.
     */
    bool isSigned() const { return false; }

    /**
     Return true if DataType is void. Otherwise return false.
     */
    bool isVoid() const { return false; }

};

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isFloatingPoint()
 */
template <> bool SpecifiedPrimitiveDataType<float>::isFloatingPoint() const;
template <> bool SpecifiedPrimitiveDataType<double>::isFloatingPoint() const;

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isSigned()
*/
template <> bool SpecifiedPrimitiveDataType<char>::isSigned() const;
template <> bool SpecifiedPrimitiveDataType<short>::isSigned() const;
template <> bool SpecifiedPrimitiveDataType<int>::isSigned() const;
template <> bool SpecifiedPrimitiveDataType<long>::isSigned() const;
template <> bool SpecifiedPrimitiveDataType<long long>::isSigned() const;

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isVoid()
*/
template <> bool SpecifiedPrimitiveDataType<void>::isVoid() const;

/*
 Template specializations for SpecifiedPrimitiveDataType<char>::printValue()
*/
template <> void SpecifiedPrimitiveDataType<char>::printValue(std::ostream &s, void *address ) const;
template <> void SpecifiedPrimitiveDataType<unsigned char>::printValue(std::ostream &s, void *address ) const;

/*
 Template specializations for SpecifiedPrimitiveDataType<void>
*/
template <> size_t SpecifiedPrimitiveDataType<void>::getSize() const;
template <> void* SpecifiedPrimitiveDataType<void>::createInstance(unsigned int num) const;
template <> void SpecifiedPrimitiveDataType<void>::deleteInstance(void* address) const;
template <> void SpecifiedPrimitiveDataType<void>::clearValue(void * address) const;
template <> bool SpecifiedPrimitiveDataType<void>::assignValue(void * address, Value * value) const;
template <> void SpecifiedPrimitiveDataType<void>::printValue(std::ostream &s, void *address ) const;
template <> Value * SpecifiedPrimitiveDataType<void>::getValue(void *address) const;



typedef SpecifiedPrimitiveDataType<void> VoidDataType;
typedef SpecifiedPrimitiveDataType<int> IntDataType;
typedef SpecifiedPrimitiveDataType<unsigned int> UnsignedIntDataType;
typedef SpecifiedPrimitiveDataType<void> VoidDataType; 
typedef SpecifiedPrimitiveDataType<char> CharDataType; 
typedef SpecifiedPrimitiveDataType<short> ShortDataType; 
typedef SpecifiedPrimitiveDataType<long> LongDataType; 
typedef SpecifiedPrimitiveDataType<long long> LongLongDataType; 
typedef SpecifiedPrimitiveDataType<unsigned char> UnsignedCharDataType; 
typedef SpecifiedPrimitiveDataType<unsigned short> UnsignedShortDataType; 
typedef SpecifiedPrimitiveDataType<unsigned int> UnsignedIntDataType; 
typedef SpecifiedPrimitiveDataType<unsigned long> UnsignedLongDataType; 
typedef SpecifiedPrimitiveDataType<unsigned long long> UnsignedLongLongDataType; 
typedef SpecifiedPrimitiveDataType<float> FloatDataType; 
typedef SpecifiedPrimitiveDataType<double> DoubleDataType; 


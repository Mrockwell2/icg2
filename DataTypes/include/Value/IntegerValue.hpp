#ifndef INTEGER_VALUE_H
#define INTEGER_VALUE_H

#include "NumericValue.hpp"

/**
 @brief IntegerValue is a Value that represents an integer value on the right-hand-side of an equation.
 */
class IntegerValue : public NumericValue {

    public:

    /**
     Constructor.
     @param value The value of this IntegerValue.
     */
    IntegerValue(long long value);

    /**
     Get the value of the IntegerValue as floating point.
     @return the value as a double.
     */
    double getFloatingPointValue() const;

    /**
     Get the value of this IntegerValue as an integer.
     @return the value as a long long int.
     */
    long long getIntegerValue() const;

    /**
     Print this FloatingPointValue to a stream.
     */
    void print(std::ostream &s) const;

    /**
     Get a string representation of this IntegerValue.
     */
    std::string toString() const;

    inline ValueType getValueType() const override { return ValueType::INTEGER; }


    private:
    long long value;
};
#endif

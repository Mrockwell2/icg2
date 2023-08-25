#ifndef FLOATING_POINT_VALUE_H
#define FLOATING_POINT_VALUE_H

#include "NumericValue.hpp"

/**
 */
class FloatingPointValue : public NumericValue {

    public:

    /**
     Constructor.
     @param value The value of this FloatingPointValue.
     */
    FloatingPointValue(double value);

    /**
     Get the value of this FloatingPointValue as floating point.
     @return the value as a double.
     */
    double getFloatingPointValue() const override;

    /**
     Get the value of this FloatingPointValue as an integer.
     @return the value as a long long int.
     */
    long long getIntegerValue() const override;

    /**
     Print this FloatingPointValue to a stream.
     */
    void print(std::ostream &s) const override;

    /**
     Get a string representation of this FloatingPointValue.
     */
    std::string toString() const override;

    inline ValueType getValueType() const override { return ValueType::FLOATING_POINT; }


    private:
    double value;
};
#endif

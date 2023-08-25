#ifndef POINTER_VALUE_H
#define POINTER_VALUE_H

#include "Value/Value.hpp"

/**
 */
class PointerValue : public Value {

    public:

    /**
     Constructor.
     @param value The value of this PointerValue.
     */
    PointerValue(void * value);

    /**
     Get the value of this PointerValue.
     @return the value of this PointerValue.
     */
    void * getPointer();

    /**
     Print this PointerValue to a stream.
     */
    void print(std::ostream &s) const override;

    /**
     Get a string representation of this PointerValue.
     */
    std::string toString() const override;

    inline ValueType getValueType() const override { return ValueType::POINTER; }

    private:
    void* value;
};
#endif

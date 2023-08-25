#ifndef STRING_VALUE_H
#define STRING_VALUE_H

#include "Value/Value.hpp"

/**
 */
class StringValue : public Value {

    public:

    /**
     Constructor.
     @param value The value of this StringValue.
     */
    StringValue(const char* value);
    StringValue(std::string value);


    /**
     Print this StringValue to a stream.
     */
    void print(std::ostream &s) const override;

    /**
     Get a string representation of this StringValue.
     */
    std::string toString() const override;

    /**
     * @brief Get the Value Type object
     * 
     * @return ValueType 
     */
    inline ValueType getValueType() const override { return ValueType::STRING; }


    std::string getRawString() const;
    std::string getEscapedString() const;

    private:

    std::string value;
};

#endif

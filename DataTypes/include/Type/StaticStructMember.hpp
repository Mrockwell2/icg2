#pragma once

#include <string>

#include "StructMember.hpp"

/**
 */
class StaticStructMember : public StructMember {

public:

    /**
     Constructor.
     */
    StaticStructMember( std::string memberName,
                        void * memberAddress,
                        std::string typeSpecName );

    // Rule of 5 is default.


    /**
     * @brief Get the address of the StructMember
     * 
     * @return void* 
     */
    void * getAddress() const;

    /**
     * @brief toString
     * 
     * @return std::string 
     */
    std::string toString() const override;

private:
    void * memberAddress;
};

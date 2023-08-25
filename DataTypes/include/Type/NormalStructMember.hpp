#pragma once

#include <string>
#include "Type/StructMember.hpp"

/**
 */
class NormalStructMember : public StructMember {

public:

    /**
     Constructor.
     */
    NormalStructMember( std::string member_name,
                        int offset,
                        std::string typeSpecName );

    /**
     */
    int getOffset() const;
    
    void * getAddress(void * struct_address) const;

    /**
     Product a string representation of this NormalStructMember.
     */
    std::string toString() const override;

private:
    int byte_offset;
};

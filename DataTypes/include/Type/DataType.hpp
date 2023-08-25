#pragma once

#include <string>
#include <iostream>

#include "Algorithm/DataTypeVisitor.hpp"

class DataTypeInator;

/** @brief Abstract base class for DataTypes. */
class DataType : public std::enable_shared_from_this<DataType> {

    public:

    virtual ~DataType() {}

   /**
    * @brief Verify that all of the types that are named by this DataType or
    * subordinate DataTypes are resolvable to actual DataTypes in
    * the TypeDictionary.
    * 
    * @param dataTypeInator - type resolver machine
    * @return true if successful, false otherwise
    */
    virtual bool validate(DataTypeInator* dataTypeInator) = 0;

    /**
     * @brief Determine whether this type has already been successfully validated.
     * 
     * @return true if validated, false otherwise
     */
    virtual bool isValid() const = 0;

    /**
     * @brief The size (in bytes) of an instance of the DataType.
     * 
     * @return size_t size of this type
     */
    virtual size_t getSize() const = 0;

    /**
     * @brief Get the Type Spec Name object
     * 
     * @return std::string name of the type
     */
    virtual std::string getTypeSpecName() const = 0;

    /**
     * @brief Create one or more instances of this type
     * 
     * @param n number to create
     * @return void* address of the beginnning of the allocation
     */
    virtual void* createInstance(unsigned int n = 1 ) const = 0;

    /**
     * @brief Delete this instance
     * 
     * @param address of instance to delete
     * @note Is this compatible with the createInstance?
     */
    virtual void deleteInstance(void* address) const = 0;

    /**
    Generate a declaration of this DataType.
    @param declarator A declarator, which can be a
        1) a variable name or an array declarator (both are direct-declarators) or
        2) a pointer declarator ( not a direct-declarator ).
    */
    virtual std::string makeDeclaration(std::string declarator) const;

    /**
    Generate a definition of this DataType.
    */
    virtual std::string toString() const;

    /* 
    Implement a Visitor Pattern
    */
    virtual bool accept (DataTypeVisitor* visitor) const = 0;


    private:
};

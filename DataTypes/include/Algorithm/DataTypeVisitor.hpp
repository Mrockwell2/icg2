#pragma once

#include <memory>

// Abstract class for a DataTypeVisitor

class DataType;
class CompositeDataType;
class ArrayDataType;
class PointerDataType;
class EnumDataType;
class StringDataType;
class PrimitiveDataType;
class SequenceDataType;

/**
 * @brief Abstract base class for a DataTypeVisitor
 * 
 */
class DataTypeVisitor {
    public:
        virtual bool visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) = 0;
        virtual bool visitEnumeratedType(std::shared_ptr<const EnumDataType> node) = 0;
        virtual bool visitCompositeType(std::shared_ptr<const CompositeDataType> node) = 0;
        virtual bool visitArrayType(std::shared_ptr<const ArrayDataType> node) = 0;
        virtual bool visitPointerType(std::shared_ptr<const PointerDataType> node) = 0;
        virtual bool visitStringType (std::shared_ptr<const StringDataType> node) = 0;
        virtual bool visitSequenceType(std::shared_ptr<const SequenceDataType>  node) = 0;

        virtual bool go(std::shared_ptr<const DataType> node);
};
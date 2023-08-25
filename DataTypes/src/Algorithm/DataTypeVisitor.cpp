#include "Algorithm/DataTypeVisitor.hpp"
#include "Type/DataType.hpp"

bool DataTypeVisitor::go(std::shared_ptr<const DataType> node) {
    return node->accept(this);
}

#pragma once

#include "Type/SequenceDataType.hpp"
#include <deque>

template <typename T>
void* sequence_construct (int num) {
    T* temp = (T*)calloc( num, sizeof(T));
    for (int ii=0 ; ii<num ; ii++) { new( &temp[ii]) T(); }
    return ((void *)temp);
}

// A deAllocator
// Object constructed with placement new must call the destructor explicitly (https://isocpp.org/wiki/faq/dtors#placement-new)
template <typename T>
void sequence_destruct (void* address) {
    T* temp = static_cast<T*> (address);
    temp->~T();
}

template <typename T>
class SpecifiedSequenceDataType : public SequenceDataType {
    public:

    SpecifiedSequenceDataType( std::string typeSpecifierName) : SequenceDataType(typeSpecifierName, sizeof(T), sequence_construct<T>, sequence_destruct<T>) {}

    // Rule of 3
    ~SpecifiedSequenceDataType() = default;
    SpecifiedSequenceDataType(const SpecifiedSequenceDataType<T>& other) = delete;
    SpecifiedSequenceDataType& operator= (SpecifiedSequenceDataType<T> rhs) = delete;

    std::vector<void *> getElementAddresses (void * address) const override {
        std::vector<void *> addresses;

        T * container = static_cast<T *> (address);

        typename T::iterator curr = container->begin();
        typename T::iterator end  = container->end();

        while (curr != end) {
            addresses.push_back(&*curr);
            curr++;
        }

        return addresses;
    }

    // TODO: template specialization to disallow resizing for std::array
    bool resize (void * address, int n_elems) const {
        T * container = static_cast<T *> (address);
        container->resize(n_elems);
        return true;
    }

    int getNumElements (void * address) const override {
        T * container = static_cast<T *> (address);
        return container->size();
    }

    // TODO: Specialize for std::array
    bool clear (void * address) const override {
        T * container = static_cast<T *> (address);
        container->clear();
        return true;
    }

};
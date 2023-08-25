
![Build Status](https://github.com/jdeans289/icg2/actions/workflows/run_tests.yml/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/jdeans289/icg2/badge.svg?branch=main)](https://coveralls.io/github/jdeans289/icg2?branch=main)

# ICG 2

A revamped Interface Code Generator and associated type aware memory management library developed for the [Trick Simulation Environment](https://github.com/nasa/trick).

## Overview

This project consists of 2 parts - the Interface Code Generator and the runtime memory management library. The general idea is that we can add reflection to C++ by generating type information, and then use that information at runtime along with a memory management library to enable checkpoint and restore of arbitrary data structures, variable lookup by name, and more. Doxygen generated documentation can be found [here](https://jdeans289.github.io/icg2/index.html), but these are still in progress.

## Interface Code Generator

The Interface Code Generator reads header files and generates data type information about the class definitions in them. This data type information is in the form of DataType object instantiations in header files named io_<header_name>.hpp 

ICG uses LibClang to understand the header files it is given. It implements a recursive AST traversal algorithm, similar to what is implemented by Clang's more sophisticated LibTooling and ASTMatchers. ICG only cares about class/struct definitions, class template definitions, and typedefs (or does it?). It gathers this information into an intermediate representation, and then uses that to create a header file io_<header_name>.hpp that instantiates DataType objects based on the information that was gathered by using LibClang. This file is generated using a custom built template engine, contained in `ICGTemplateEngine/`. 

The choice to use LibClang comes from the Trick project's requirement to support many versions of Clang/LLVM. The previous ICG uses a mix of LLVM internals and LibTooling. It is littered with #ifdef statements to support various API changes in libclang, requiring constant maintenance. Part of the goal of this project is to eliminate that pain point for future generations of Trick developers and users. 

## DataTypes

The DataType is the representation we use to understand type structure at runtime. DataType is the base class of a hierarchy of types that can be used to represent all supported types. The derived classes include:
- PrimitiveDataType
- CompositeDataType
- StringDataType
- ArrayDataType
- PointerDataType
- SequenceType

The DataType interface provides some basic operations over types that can be used to implement more complex algorithms. Includes `getTypeName()`, `getSize()`, the ability to allocate and delete an instance, and the ability to get and set the value given an address (using the `Value` class hierarchy).

DataTypes also support a Visitor pattern with double dispatch. Every DataType has an `accept (DataTypeVisitor *) -> bool` method which should be used to call the correct method in the `DataTypeVisitor` class. These visitors are used to implement checkpoint and lookup algorithms.

The entity responsible for managing the types is the `DataTypeInator`. The DataTypeInator holds a TypeDictionary, which tracks all BaseTypes. The DataTypeInator's most important method is `resolve(string) -> DataType *`. This will either apply a modifier type or lookup a base type in the type dictionary, and return the appropriate type.

This requires runtime memory management. All allocations must be allocated through or registered with the MemoryManager. The memory manager tracks allocations using a class called `AllocInfo`, which contains address, name, size, and `DataType*` of a particular allocation. 

## Supported Types

Currently, the types supported are

- Primitives:
```
void
char
short
int
long
wchar_t
long
unsigned
unsigned char
unsigned short
unsigned int
unsigned long
float
double
```
- std::string
- Constrained Arrays
- Pointers
- User Defined classes/structs with members of any of the above types
- Namespaces
- Nested classes
- STL sequences (vector, list, deque)
- Any mix of the above

See the issue board for in progress work.

## Clone

Use a recursive clone to include the repository's submodules

```
git clone --recursive git://github.com/jdeans289/icg2.git
```

## Build

Use the normal CMake process to build.

```
mkdir build
cd build
cmake ..
make -j16
```

Tests are built along with the normal build process (should probably separate that out later).

Run tests through ctest (or make test)
```
cd build
ctest
```

## Usage

See the `test/` directory for examples of intended usage. 

The interface of the MemoryManager is intended to be (nearly?) identical to the Trick Project's [Memory Manager](https://nasa.github.io/trick/documentation/simulation_capabilities/memory_manager/MemoryManager), to allow for a smooth transition. Users of Trick should not have to update their simulations when this library is eventually integrated into Trick. The integration into `trick-CP` and excision of the old `ATTRIBUTES` (analogue of DataType)  and `REF2` (AllocInfo + specific reference instance info, like units) will require a lot of work from Trick developers. This integration is planned for Trick 23. 



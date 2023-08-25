#ifndef ALLOCINFO_HH
#define ALLOCINFO_HH

#include <stdexcept>
#include <vector>
#include <string>
#include <memory>

class DataType;
class CompositeDataType;
class ArrayDataType;
class DataTypeInator;
class CheckpointAgent;
class MemoryManager;
class LexicalAnalyzer;
struct VarAccessInfo;


// FIXME: the follow define should be in a more appropriate place.
// #define TRICK_MAX_INDEX 8

namespace StorageClass {
    enum e {
        LOCAL  = 0,
        EXTERN = 1
    };
};

/**
 * The AllocInfo class describes a chunk of memory that the MemoryManager knows about.
 */

class AllocInfo {

    public:
    AllocInfo( const std::string& varName,
            std::shared_ptr<const DataType> type,
            void*            suppliedAllocation = 0)  ;

    // Delete the copy constructor.
    AllocInfo(const AllocInfo& other) = delete;

    /**
      AllocInfo destructor.
      @note This also deletes the underlying allocation if it was allocated locally.
     */

    ~AllocInfo();

    /**
     @return the name given to the allocation.
     Tested in: AllocInfoTest::getters1 AllocInfoTest::getters2.
     */
    std::string getName() const;

    std::string getSerialName() const;
    unsigned int getSerialNumber() const;

    /**
     @return the starting address of the allocation.
     Tested in: AllocInfoTest::getters1, AllocInfoTest::getters2.
     */
    void * getStart() const;

    /**
     @return the size of the allocation in bytes.
     Tested in: AllocInfoTest::getters1, AllocInfoTest::getters2.
     */
    int getSize() const;

    /**
     @return the type specifier.
     Tested in: AllocInfoTest::getters1, AllocInfoTest::getters2.
     */
    std::string getTypeSpecifierName() const;

    /**
     @return the number of dimensions.
     Tested in: AllocInfoTest::getters1, AllocInfoTest::getters2.
     */
    unsigned int getDimensionsCount() const;

    /**
     @param n index of the dimension.
     @return the size of the nth dimension.
     Tested in: AllocInfoTest::getters1, AllocInfoTest::getters2.
     */
    int getDimensionSize( unsigned int n ) const;

    /**
     @return the DataType for the allocation.
     */
    std::shared_ptr<const DataType> getDataType() const;

    /**
     @return Get 
     */
    StorageClass::e getStorageClass() const;

    /**
     @brief Clear the every element of the allocation to 0.
     Tested in: AllocInfoTest::clear.
     */
    void clear() const;


    /**
     */
    // void* resize( size_t newElementCount );

    /**
     @return true, if the allocation contains the given address. Otherwise return false.
     */
    bool contains(void* address) const;

    /**
     */
    bool getVarAccessInfo( LexicalAnalyzer* lexer, VarAccessInfo& varAccessInfo );


    std::string toString() const;


    private:

    void initialize( const std::string& varName,
                     std::shared_ptr<const DataType> type,
                     void*              suppliedAllocation )  ;

    std::string     name;                        /**< ** Object name */
    void*           start;                       /**< ** Address where this allocation starts */
    void*           end;                         /**< ** Address where this allocation ends */

    std::shared_ptr<const DataType> dataType;                    /**< **  */

    StorageClass::e storageClass;                /**< ** Storage class */

    unsigned int serialNumber;
    static unsigned int nextSerialNumber;

    static const std::string localPrefix;
    static const std::string externPrefix;
} ;
#endif

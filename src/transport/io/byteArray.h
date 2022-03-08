#pragma once

#include <iostream>
#include <iomanip>

#include <cstring>
#include <memory>

#include "../fw/modelKey.h"
#include "./IModel.h"

namespace Quix { namespace Transport {




/**
 * Class representing the Array segment with its length and start
 * 
 * Note: public API is prepared to be used for potential ArraySegment implementation
 */
class ByteArray : public IModel{

private:
    size_t len_;
    size_t start_;
    std::shared_ptr<uint8_t> data_;

public:
    /**
     *  Initialize array from the array data
     * 
     *  @param data shared pointer containing the pointer to the data array
     *  @param len length of the segment inside data array
     *  @param start index representing start of data segment inside the data array
     */
    inline ByteArray(
        const std::shared_ptr<uint8_t> data, 
        size_t len, 
        size_t start = 0
    )
     :  
     len_(len), 
     start_(start), 
     data_(data) 
    {
        
    };

    /**
     *  Initialize array from the string of data
     * 
     *  @param data string containing the data array
     */
    inline ByteArray(const std::string& data)
     :
     len_(data.size()),
     start_(0),
     //initialize array of same size as std::string
     data_(std::shared_ptr<uint8_t>(new uint8_t[data.size()], std::default_delete<uint8_t[]>())) 
    {
        //copy data from string to the array
        memcpy((void*)&(*data_), data.c_str(), len_);
    };

    /**
     *  Initialize empty ( zero length ) array
     */
    inline ByteArray()
     : 
     len_(0), 
     start_(0), 
     data_(nullptr)
    {
        
    };

    /**
     *  Copy constructor
     */
    inline ByteArray(const ByteArray& other) = default;

    /**
     *  Initialize the array from other array but with different from and length indexes
     */
    inline ByteArray(
        const ByteArray& other, 
        size_t len,
        size_t start
    )
     : 
     ByteArray(other.rawdata(), len, start)
    {

    };

    /**
     *  Initialize the empty array of size len
     * 
     *  Note: this function does not reset the content of array
     */
    static inline ByteArray initEmpty(size_t len)
    {
        auto ret = ByteArray(
            std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
            len
        );

        auto rawArray = ret.data();
        for( int i = 0; i < len; ++i )
        {
            rawArray[i] = 0;
        }

        return ret;
    };

    /**
     *  Initialize the array filled in with random bytes of size len
     */
    static inline ByteArray initFromArray(uint8_t* inp, size_t len)
    {
        auto ret = ByteArray::initEmpty(len);
        memcpy(ret.data(), inp, len * sizeof(uint8_t));
        return ret;
    };

    /**
     *  Initialize the array filled in with random bytes of size len
     */
    static inline ByteArray initRandom(size_t len)
    {
        auto ret = ByteArray::initEmpty(len);
        auto rawArray = ret.data();
        for( int i = 0; i < len; ++i )
        {
            rawArray[i] = rand() % ( UINT8_MAX + 1 );
        }
        return ret;
    };


    /**
     *  Concatenate one chunk with before the rest of array ( e.g. (HEADER->REST) )
     * 
     *  @param headerArray pointer to the beginning of the data chunk to be prepended
     *  @param headerLength size of the data chunk to be prepended
     *  @param rest ByteArray chunk to be places after the header
     * 
     *  @return new ByteArray containing the two contatenated pieces
     *  Note: this function does not reset the content of array
     */
    static inline ByteArray prependHeader(
        uint8_t* headerArray, 
        size_t headerLength, 
        const ByteArray& rest
    ) 
    {
        const auto restLen = rest.len();

        //initialize new array
        auto array = ByteArray::initEmpty(headerLength + restLen);
        //copy header chunk to new array
        memcpy(&*array.data(), headerArray, headerLength);
        //copy rest chunk to new array
        memcpy(&*array.data() + headerLength, rest.data(), restLen);
        return array;
    };

    /**
     * Get Model Key definition of the ByteArray model
     *  this is implemented to satisty IModel interface
     *  so the ByteArray can be loaded inside Package
     */
    inline const ModelKey modelKey() const 
    { 
        return ModelKey("ByteArrayPackage");
    };

    /**
     * Compare to another ByteArray segment
     */
    inline const bool operator==(const ByteArray& other) const { 
        return 
            len_ == other.len() 
                && 
            (len_ <= 0 || memcmp(data(), other.data(), len_) == 0)
            ;
    }

    /**
     * Assign operator
     */
    inline ByteArray& operator=(const ByteArray& other) { 
        if( this == &other ){
            return *this;
        }

        this->len_ = other.len_;
        this->start_ = other.start_;
        this->data_ = other.data_;

        return *this;
    }

    /**
     * Get length of the array segment
     */
    inline const size_t len() const 
    { 
        return len_; 
    }

    /**
     * Get all data of array segment inside one piece of array
     */
    inline uint8_t*const data() const 
    { 
        return &(*data_) + start_;
    }

    /**
     * Get pointer to first character of the data segment
     * 
     * NOTE: does not guaranee that the pointer contains whole segment of array
     */
    inline uint8_t*const begin() const 
    { 
        return &(*data_) + start_;
    }

    /**
     * Get pointer to the beginning of underlying data array
     */
    inline const std::shared_ptr<uint8_t> rawdata() const 
    { 
        return data_;
    }

    /***
     * Implement << operator for printing into the std::ostream
     */
    friend auto operator<<(std::ostream& os, ByteArray const& m) -> std::ostream& { 
        os << " ByteArray("<<m.len()<<")<<";

        os << std::hex << std::setfill('0') << std::setw(2);

        auto data = m.data();
        for( auto i = 0; i < m.len(); i++ ){
            if ( data[i] == '\0' ) { os << '0'; }
            os << (int)(data[i]) << " ";
        }

        os << std::dec << std::setfill('0') << std::setw(0);

        return os << ">>";
    }
};

} }
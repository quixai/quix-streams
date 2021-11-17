#pragma once

#include <functional>
#include <memory>

#include <algorithm>

#include "../io/package.h"
#include "../io/IPublisher.h"

#include "./byteSplitProtocolHeader.h"

namespace Quix { namespace Transport {

/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteSplitter{

private:
    class Iterator : public std::iterator<
                        // std::input_iterator_tag,   // iterator_category
                        // long,                      // value_type
                        // long,                      // difference_type
                        // const long*,               // pointer
                        // long                       // reference
                        std::input_iterator_tag,   // iterator_category
                        std::shared_ptr<ByteArrayPackage>,                      // value_type
                        long,                      // difference_type
                        std::shared_ptr<ByteArrayPackage>,               // pointer
                        std::shared_ptr<ByteArrayPackage>                       // reference
                                      >
    {
    public:
        // Iterator constructors here...
        Iterator(
            std::shared_ptr<ByteArrayPackage> originalPackage, 
            size_t maxMessageSizeWithoutHeader, 
            uint32_t messageId, 
            uint8_t curIndex = 0
        );

        reference operator*() const;
        // pointer operator->();

        // Prefix increment
        Iterator& operator++() { curIndex_++; return *this; }  

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.curIndex_ == b.curIndex_; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.curIndex_ != b.curIndex_; };     

    private:
        std::shared_ptr<ByteArrayPackage> originalPackage_;
        const size_t maxMessageSizeWithoutHeader_;
        uint8_t curIndex_;
        uint32_t messageId_;
    };

    uint32_t messageId;
    ///max length of message including header
    size_t maxMessageSize_;
    ///max length of message without header
    size_t maxMessageSizeWithoutHeader_;

public:
    /**
     * Initializes a new instance of ByteSplitter
     * 
     * @param maxMessageSize The maximum message size
     */
    ByteSplitter(const size_t maxMessageSize);

    /**
     * Get maximum message size of packet
     */
    const size_t absoluteMaxMessageSize() const;

    Iterator begin(std::shared_ptr<ByteArrayPackage> package);

    Iterator end(std::shared_ptr<ByteArrayPackage> package) const;

    std::vector<std::shared_ptr<ByteArrayPackage>> split(std::shared_ptr<ByteArrayPackage> package);

};

} }
#pragma once

#include <string>

namespace Quix { namespace Transport {

/**
 * Class representing the unique key of the Codec.
 * 
 * Useful to differentiate different codecs for the same type when there are
 *  multiple defined.
 *  For example, if a type has both JSON and Protobuf, the Ids would be 'JSON' and 'Protobuf'.
 *  Note, you can, but do not need to use the type's full name. For serialization purposes it is better to use as short
 *  as possible yet unique Ids.
 */
class CodecId : public std::string {
public:
    /**
     * Initialize with the string representation of the key.
     * 
     * @param codecId string representation of the key ( default "UNKNOWN" ). Default value to satisfy usage in std containers.
     */
    inline CodecId(const std::string& codecId = "UNKNOWN"):std::string(codecId){};
    inline CodecId(const CodecId& codecId) = default;
};

} }
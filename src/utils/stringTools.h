#pragma once

#include <string>
#include <algorithm>

namespace Quix { 

class StringTools{
    static uint32_t rand32()
    {
        return ((rand() & 0x3) << 30) | ((rand() & 0x7fff) << 15) | (rand() & 0x7fff);
    }
    static bool gen_uuid4(char* dst, size_t len)
    {
        int n = snprintf(dst, len, "%08x-%04x-%04x-%04x-%04x%08x", 
            StringTools::rand32(),                         // Generates a 32-bit Hex number
            StringTools::rand32() & 0xffff,                // Generates a 16-bit Hex number
            ((StringTools::rand32() & 0x0fff) | 0x4000),   // Generates a 16-bit Hex number of the form 4xxx (4 indicates the UUID version)
            (StringTools::rand32() & 0x3fff) + 0x8000,     // Generates a 16-bit Hex number in the range [0x8000, 0xbfff]
            StringTools::rand32() & 0xffff, StringTools::rand32());     // Generates a 48-bit Hex number
            
        return n >= 0 && n < len;             // Success only when snprintf result is a positive number and the provided buffer was large enough.
    }


public:
    inline static bool isEmptyOrWhiteSpace(const std::string& s)
    {
        return s.size() <= 0 || std::all_of(s.begin(),s.end(),[](unsigned char c){ return std::isspace(c); });
    }
    inline static std::string genUuid()
    {
        char c[37];
        gen_uuid4((char*)&c, 37);
        return std::string(c);
    }
};

}
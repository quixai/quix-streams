#pragma once

#include <string>
#include <iostream>

namespace Quix { 


/****
 * Boxing type for allowing dynamic typed values
 */

class BoxedValue {

    int64_t lvalue_;
    std::string svalue_;

    enum Type{ Undefined, String, Long };

    Type type_;
public:

    BoxedValue( )
    :
    type_(Type::Undefined)
    {
    }

    BoxedValue(std::string s)
    :
    type_(Type::String),
    svalue_(s)
    {

    }

    BoxedValue(const char* s)
    :
    type_(Type::String),
    svalue_(s)
    {

    }

    BoxedValue(int64_t v)
    :
    type_(Type::Long),
    lvalue_(v)
    {

    }

    BoxedValue( const BoxedValue& ) = default;

    inline bool isString() const { return type_ == Type::String; }
    inline bool isLong() const { return type_ == Type::Long; }

    inline std::string stringValue( ) const { 
        return svalue_;
    }

    inline int64_t longValue( ) const { 
        return lvalue_;
    }

    inline bool tryGet(std::string& s) const { 
        if( !isString() ) return false;

        s = svalue_;
        return true;
    }

    inline bool tryGet(int64_t& l) const { 
        if( !isLong() ) return false;

        l = lvalue_;
        return true;
    }

    inline bool operator<(const BoxedValue& other) const
    {
        if( type_ != other.type_ )
        {
            return type_ < other.type_;
        }

        switch( type_ )
        {
            case Type::String:
                return svalue_ < other.svalue_;
            case Type::Long:
                return lvalue_ < other.lvalue_;
            case Type::Undefined:
                return false;
            default:
                throw std::exception();
        }
    }

    inline bool operator==(const BoxedValue& other) const
    {
        if( type_ != other.type_ )
        {
            return false;
        }

        switch( type_ )
        {
            case Type::String:
                return svalue_ == other.svalue_;
            case Type::Long:
                return lvalue_ == other.lvalue_;
            case Type::Undefined:
                return true;
            default:
                throw std::exception();
        }
    }

    inline bool operator!=(const BoxedValue& other) const
    {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const BoxedValue& tc)
    {
        switch( tc.type_ )
        {
            case Type::String:
                os << tc.svalue_.c_str();
                break;
            case Type::Long:
                os << tc.lvalue_;
                break;
            case Type::Undefined:
                os << "???";
                break;
            default:
                throw std::exception();
        }
        return os;
    };

};

}
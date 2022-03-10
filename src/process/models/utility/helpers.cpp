#include <algorithm>

#include "./helpers.h"

using namespace std;
using namespace Quix::Process;

std::string Helpers::formatLocation(const std::string& location)
{
    auto toTrimFun = [](unsigned char ch) {
        return !( std::isspace(ch) || ch == '/' );
    };

    auto begin = std::find_if( location.begin(), location.end(), toTrimFun );
    auto end   = std::find_if( location.rbegin(), location.rend(), toTrimFun ).base();

    int len = end - begin + 1;
    // if( len == location.size() ) { return location; }   //nothing to trim

    std::string ret;
    ret.reserve(len);
    ret.push_back( '/' );
    ret.append( begin, end );

    return ret;
}

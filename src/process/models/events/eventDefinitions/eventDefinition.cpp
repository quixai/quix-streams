#include <algorithm>

#include "../../../../exceptions/argumentOutOfRangeException.h"

#include "./eventDefinition.h"


using namespace std;
using namespace Quix;
using namespace Quix::Process;

void EventDefinition::setId( const string& i ) { 
    if( 
        std::find_if( 
            i.cbegin(), 
            i.cend(), 
            [](const unsigned char ch) {
                return ch == '\\' || ch == '/';
            } 
        ) != i.cend() 
    )
    {
        throw ArgumentOutOfRangeException("Parameter Id must not contain the following characters: /\\");
    }
    
    id_ = i;
}

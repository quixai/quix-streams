#pragma once

#include <map>
#include <string>

namespace Quix { namespace Transport {

/**
 * This is read only meta data context meant to use for providing extra context for the the value
 * 
 * TODO: make readonly
 */
class MetaData : public std::map<std::string, std::string>{ };

} }
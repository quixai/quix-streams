#pragma once

#include <mutex>
#include <functional>
#include <memory>
#include <regex>

#include "../transport/io/IPublisher.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/package.h"

namespace Quix { namespace Transport { namespace Kafka  {

class KeepAliveMessage : public ByteArray{
public:
    KeepAliveMessage( ) { 
    }

    const ModelKey modelKey() const {
        return Quix::Transport::ModelKey::forType<KeepAliveMessage>();
    };

};


/**
 * Interface for providing a class a way to push Package to listener
 */
class Constants{

public:

    static std::shared_ptr<Quix::Transport::IPackage> keepAlivePackage;
    static std::regex exceptionMsRegex;

};

} } }
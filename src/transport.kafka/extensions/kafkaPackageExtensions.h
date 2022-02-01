#pragma once

#include <mutex>
#include <chrono>
#include <functional>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "../../utils/timer.h"

#include "../../transport/io/IPublisher.h"
#include "../../transport/io/IPackage.h"
#include "../../transport/io/byteArray.h"
#include "../IKafkaPublisher.h"
#include "../topicConfiguration.h"
#include "../knownKafkaTransportContextKeys.h"

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 */
template<class T>
class KafkaPackageExtensions {

    T* const package_;

public:

    KafkaPackageExtensions(T* const package)
    :
    package_(package)
    {

    }

    bool tryGetKey( std::string& out )
    {
        auto& transportContext = package_->transportContext();

        auto it = transportContext->find( KnownKafkaTransportContextKeys::Key );
        if( it != transportContext->end() ) {
            if( it->second.tryGet(out) )
            {
                return true;
            }
            return true;
        }

        return false;
    }

    void setKey( std::string& value )
    {
        package_->transportContexts()[ KnownKafkaTransportContextKeys::Key ] = value;
    }

    bool isKeepAlivePackage( )
    {
        std::string key;
        if ( !tryGetKey( key ) ) { return false; }
        
        std::string keepAliveKey;
        if ( !KafkaPackageExtensions<Quix::Transport::IPackage>( Constants::keepAlivePackage.get() ).tryGetKey( keepAliveKey ) ) { return false; }

        return key == keepAliveKey;
    }

};


} } }
// A2DD.cpp
#include "transport.h"
#include "./transportPublisher.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

    TransportPublisher::TransportPublisher(IPublisher* input, ByteSplitter* byteSplitter)
    {
        if(byteSplitter != nullptr)
        {
            //pipe serializer into splitter and then to the input
            serializer.onNewPackage = std::bind( &ByteSplitter::send, byteSplitter, std::placeholders::_1 );
            byteSplitter->onNewPackage = std::bind( &IPublisher::send, input, std::placeholders::_1 );
        }
        else
        {
            //pipe the serializer directly to the input
            serializer.onNewPackage = std::bind( &IPublisher::send, input, std::placeholders::_1 );
        }

    }

    void TransportPublisher::send(std::shared_ptr<IPackage> package) const
    {
        //TODO: cancellationToken
        return serializer.send(package);
    }

} }
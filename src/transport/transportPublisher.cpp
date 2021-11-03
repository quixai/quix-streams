// A2DD.cpp
#include "transport.h"
#include "./transportPublisher.h"
#include "./io/abstractPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

    TransportPublisher::TransportPublisher(AbstractInput* input, ByteSplitter* byteSplitter)
    {
        if(byteSplitter != nullptr)
        {
            //pipe serializer into splitter and then to the input
            serializer.onNewPackage = std::bind( &ByteSplitter::send, byteSplitter, std::placeholders::_1 );
            byteSplitter->onNewPackage = std::bind( &AbstractInput::send, input, std::placeholders::_1 );
        }
        else
        {
            //pipe the serializer directly to the input
            serializer.onNewPackage = std::bind( &AbstractInput::send, input, std::placeholders::_1 );
        }

    }

    void TransportPublisher::send(AbstractPackage* package)
    {
        //TODO: cancellationToken
//        if (cancellationToken.IsCancellationRequested) return Task.FromCanceled(cancellationToken);
        return serializer.send(package);
    }

} }
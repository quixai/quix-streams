// A2DD.cpp
#include "transport.h"
#include "./transportPublisher.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

    TransportPublisher::TransportPublisher(IPublisher* input, ByteSplitter* byteSplitter)
    :
    byteSplittingModifier_(byteSplitter)
    {
        if(byteSplitter != nullptr)
        {
            //pipe serializer into splitter and then to the input
            serializingModifier_.onNewPackage += std::bind( &ByteSplittingModifier::send, &byteSplittingModifier_, std::placeholders::_1 );
            byteSplittingModifier_.onNewPackage += std::bind( &IPublisher::send, input, std::placeholders::_1 );
        }
        else
        {
            //pipe the serializer directly to the input
            serializingModifier_.onNewPackage += std::bind( &IPublisher::send, input, std::placeholders::_1 );
        }

    }

    void TransportPublisher::send(std::shared_ptr<IPackage> package)
    {
        //TODO: cancellationToken
        return serializingModifier_.send(package);
    }

} }
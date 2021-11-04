// A2DD.cpp
#include "transport.h"
#include "./transportSubscriber.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

//     TransportWriter::TransportWriter(AbstractInput* input, AbstractByteSplitter* byteSplitter)
//     {
//         if(byteSplitter != nullptr)
//         {
//             //pipe serializer into splitter and then to the input
//             serializer.onNewPackage = std::bind( &AbstractByteSplitter::send, input, std::placeholders::_1 );
//             byteSplitter->onNewPackage = std::bind( &AbstractInput::send, input, std::placeholders::_1 );
//         }
//         else
//         {
//             //pipe the serializer directly to the input
//             serializer.onNewPackage = std::bind( &AbstractInput::send, input, std::placeholders::_1 );
//         }

//     }

//     void TransportWriter::send(IPackage* package)
//     {
//         //TODO: cancellationToken
// //        if (cancellationToken.IsCancellationRequested) return Task.FromCanceled(cancellationToken);
//         return serializer.send(package);
//     }

} }
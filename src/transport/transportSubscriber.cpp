// A2DD.cpp
#include "transport.h"
#include "./transportSubscriber.h"
#include "./fw/deserializingModifier.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

    TransportSubscriber::TransportSubscriber(ISubscriber* subscriber)
    {
        std::vector<void*> outputsAndInputs;

        // output -> merger -> deserializer -> commitModifier -> raise
        outputsAndInputs.push_back(subscriber);
        outputsAndInputs.push_back((void*)&deserializingModifier_);

        // Now that we have the modifier, lets connect them up
        auto previous = outputsAndInputs[0];
        for (int index = 1; index < outputsAndInputs.size(); index++)
        {
            auto modifier = outputsAndInputs[index];
            ((ISubscriber*)previous)->onNewPackage = std::bind( &IPublisher::send, (IPublisher*)modifier, std::placeholders::_1 );
            previous = modifier;
        }

        // Connect last output to TransportSubscriber (this class)
        ((ISubscriber*)previous)->onNewPackage = std::bind( &TransportSubscriber::sendInternal, this, std::placeholders::_1 );

    }

    void TransportSubscriber::sendInternal(std::shared_ptr<IPackage> package){
        onNewPackage(package);
    }

} }
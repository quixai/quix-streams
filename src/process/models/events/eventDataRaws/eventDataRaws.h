#pragma once

#include <vector>

#include "./eventDataRaw.h"

#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class EventDataRaws : public Quix::Transport::IModel
    {
    public:

        std::vector<EventDataRaw> events;

        /**
         * Get Model Key definition of the ByteArray model
         *  this is implemented to satisty IModel interface
         *  so the ByteArray can be loaded inside Package
         */
        inline const Quix::Transport::ModelKey modelKey() const 
        { 
            return Quix::Transport::ModelKey::forType<EventDataRaws>();
        };
    };
} }
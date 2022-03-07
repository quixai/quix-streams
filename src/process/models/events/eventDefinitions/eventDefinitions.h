#pragma once

#include <vector>

#include "./eventDefinition.h"

#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class EventDefinitions : public Quix::Transport::IModel
    {
    public:

        /// Events at root level
        std::vector<EventDefinition> events;

        /**
         * Get Model Key definition of the ByteArray model
         *  this is implemented to satisty IModel interface
         *  so the ByteArray can be loaded inside Package
         */
        inline const Quix::Transport::ModelKey modelKey() const 
        { 
            return Quix::Transport::ModelKey::forType<EventDefinitions>();
        };
    };
} }
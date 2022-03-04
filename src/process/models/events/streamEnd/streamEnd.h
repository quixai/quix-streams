#pragma once

#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class StreamEnd : public Quix::Transport::IModel
    {
    public:
        enum StreamEndType{
            Closed = 0,
            Aborted = 1,
            Terminated = 2
        };

        /**
         * Get Model Key definition of the ByteArray model
         *  this is implemented to satisty IModel interface
         *  so the ByteArray can be loaded inside Package
         */
        inline const Quix::Transport::ModelKey modelKey() const 
        { 
            return Quix::Transport::ModelKey::forType<StreamEnd>();
        };

        StreamEndType streamEndType = StreamEndType::Closed;
    };
} }
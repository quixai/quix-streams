#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <map>

#include "../../../utils/optional.hpp"

#include "../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class StreamProperties : public Quix::Transport::IModel
    {
    public:

        /// The human friendly name of the stream
        std::string name;

        /// Specify location of the stream in data catalogue. 
        /// For example: /cars/ai/carA/.
        std::string location;

        /// Additional metadata for the stream.
        std::map<std::string, std::string> metadata;

        /// The ids of streams this stream is derived from.
        std::vector<std::string> parents;

        /// Indicates the time when data was originally recorded.
        /// This can be different than the time the data is streamed.
        std::experimental::optional< std::chrono::time_point<std::chrono::system_clock> > timeOfRecording;


        /**
         * Get Model Key definition of the ByteArray model
         *  this is implemented to satisty IModel interface
         *  so the ByteArray can be loaded inside Package
         */
        inline const Quix::Transport::ModelKey modelKey() const 
        { 
            return Quix::Transport::ModelKey::forType<StreamProperties>();
        };
    };
} }
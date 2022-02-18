#pragma once

#include <cstring>

#include "transport/io/IModel.h"
#include "transport/fw/modelKey.h"


class ExampleModel : public Quix::Transport::IModel{
    static const size_t MSG_SIZE = 100;
public:
    char msg[MSG_SIZE];

    ExampleModel( const char* msg = "" ) { 
        strncpy(this->msg, msg, MSG_SIZE - 1); 
        this->msg[MSG_SIZE - 1] = '\0';
    };

    bool operator==(const ExampleModel& other) const {
        return strcmp(msg, other.msg) == 0;
    };

    const Quix::Transport::ModelKey modelKey() const {
        return Quix::Transport::ModelKey::forType<ExampleModel>();
    };

};

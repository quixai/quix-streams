#pragma once

#include <functional>

#include "transport/io/IPackage.h"
#include "transport/io/IPublisher.h"
#include "transport/io/ISubscriber.h"


class Passthrough : public Quix::Transport::IPublisher, public Quix::Transport::ISubscriber 
{

private: 
    std::function<void(std::shared_ptr<Quix::Transport::IPackage>)> callback_ = nullptr;

public:
    Passthrough(std::function<void(std::shared_ptr<Quix::Transport::IPackage>)> callback = nullptr)
     : 
        callback_(callback) 
    {

    };

    ~Passthrough() 
    {
        
    };

    void send(std::shared_ptr<Quix::Transport::IPackage> package) const
    {
        if(callback_ != nullptr)
        {
            callback_(package);
        }
        else if(onNewPackage != nullptr)
        {
            onNewPackage(package);
        }
    }

};

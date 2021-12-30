#pragma once

#include <functional>

#include "transport/fw/ICanCommit.h"
#include "transport/io/IPackage.h"
#include "transport/io/IPublisher.h"
#include "transport/io/ISubscriber.h"


class Passthrough : public Quix::Transport::IPublisher, public Quix::Transport::ISubscriber, public Quix::Transport::ICanCommit
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

    std::vector<std::shared_ptr<Quix::Transport::TransportContext> > filterCommittedContexts
    (
        void* sender, 
        const std::vector<std::shared_ptr<Quix::Transport::TransportContext> >& contextsToFilter
    )
    {
        return contextsToFilter;
    };

    void send(std::shared_ptr<Quix::Transport::IPackage> package)
    {
        if(callback_ != nullptr)
        {
            callback_(package);
        }
        else
        {
            onNewPackage(package);
        }
    }

    void commit(const std::vector<std::shared_ptr<Quix::Transport::TransportContext>>& transportContexts)
    {
        /// TODO: maybe memory mess because of (void*) cast
        this->onCommitting(this, Quix::Transport::ICanCommit::OnCommittingEventArgs((void*)&transportContexts));
        this->onCommitted(this, Quix::Transport::ICanCommit::OnCommittedEventArgs((void*)&transportContexts));
    }

};

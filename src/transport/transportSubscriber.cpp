// A2DD.cpp
#include "transport.h"
#include "./transportSubscriber.h"
#include "./fw/deserializingModifier.h"
#include "./fw/IModifier.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>


namespace Quix { namespace Transport {

    TransportSubscriber::TransportSubscriber(ISubscriber* subscriber)
    :
    byteMergingModifier_(&this->byteMerger_)
    {

        // default variables initialization
        onFilterCommittedContexts = [](void *state, const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter){
            return std::vector<std::shared_ptr<Quix::Transport::TransportContext>>();
        };
        contextFilterByState = [](void *state, const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter){
            return contextsToFilter;
        };



        TransportSubscriberOptions options;
        //TODO: implement configureOptions

        const auto& commitOptions = options.commitOptions;

        std::vector<IModifier*> outputsAndInputs;

        auto& firstOutputAndInput = subscriber;

        // output -> merger -> deserializer -> commitModifier -> raise
//        outputsAndInputs.push_back(subscriber);
        outputsAndInputs.push_back(&byteMergingModifier_);
        outputsAndInputs.push_back(&deserializingModifier_);
        if( commitOptions.autoCommitEnabled )
        {
            commitModifier_ = new CommitModifier(commitOptions);            
            outputsAndInputs.push_back(commitModifier_);
        }

        // Now that we have the modifier, lets connect them up
//        auto previous = outputsAndInputs[0];
        auto previous = firstOutputAndInput;
        for (int index = 0; index < outputsAndInputs.size(); index++)
        {
            auto modifier = (ISubscriber*)outputsAndInputs[index];
            ((ISubscriber*)previous)->onNewPackage += std::bind( &IPublisher::send, (IPublisher*)modifier, std::placeholders::_1 );
            previous = modifier;
        }

        // Connect last output to TransportSubscriber (this class)
        ((ISubscriber*)previous)->onNewPackage += std::bind( &TransportSubscriber::sendInternal, this, std::placeholders::_1 );

        // Hook up committing modifiers from front (output) to back (this)
        ICanCommit* previousCanCommitModifier = nullptr;
        for (int index = 0; index < outputsAndInputs.size(); index++)
        {
            auto modifier = outputsAndInputs[index];
            if (previousCanCommitModifier != nullptr)
            {
                ICanCommitSubscriber* subscriber = dynamic_cast<ICanCommitSubscriber*>(modifier);
                if ( subscriber != nullptr )
                {
                    subscriber->subscribe(previousCanCommitModifier);
                }
            }

            ICanCommit* committingModifier = dynamic_cast<ICanCommit*>(modifier);
            if ( committingModifier != nullptr )
            {
                previousCanCommitModifier = committingModifier;
            }
        }

        // Connect last committing modifiers to TransportOutput (this class)
        if ( previousCanCommitModifier != nullptr )
        {
            this->onCommit_ = std::bind(&ICanCommit::commit, previousCanCommitModifier, std::placeholders::_1);

            //TODO: onFilterCommittedContexts
            this->onFilterCommittedContexts = std::bind(&ICanCommit::filterCommittedContexts, previousCanCommitModifier, std::placeholders::_1, std::placeholders::_2);
            

            // bound previousCanCommitModifier->onCommitted += this->onCommited(_1, _2)
            previousCanCommitModifier->onCommitted += std::bind(
                                                            &EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::ICanCommit::OnCommittedEventArgs &>::operator(),
                                                            this->onCommitted, 
                                                            std::placeholders::_1, 
                                                            std::placeholders::_2
                                                        );
            // bound previousCanCommitModifier->onCommitting += this->onCommiting(_1, _2)
            previousCanCommitModifier->onCommitting += std::bind(
                                                            &EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::ICanCommit::OnCommittingEventArgs &>::operator(),
                                                            this->onCommitting, 
                                                            std::placeholders::_1, 
                                                            std::placeholders::_2
                                                        );
        }



        // Hook up modifiers implementing IRevocation.... from front (output) to back (this)
        IRevocationPublisher* previousRevocationPublisher = nullptr;
        for ( int index = 0; index < outputsAndInputs.size() ; index++ )
        {
            auto modifier = outputsAndInputs[index];
            if ( previousRevocationPublisher != nullptr )
            {
                // If we have previous notification modifier then try to hook up callbacks to it. Without previous notification modifier
                // there would be nothing to hook up to.
                IRevocationSubscriber* subscriber = dynamic_cast<IRevocationSubscriber*>(modifier);
                if ( subscriber != nullptr )
                {
                    subscriber->subscribe( previousRevocationPublisher );
                }
            }
            
            // assign current as previous for next loop
            IRevocationPublisher* revocationPublisher = dynamic_cast<IRevocationPublisher*>(modifier);
            if ( revocationPublisher != nullptr )
            {
                previousRevocationPublisher = revocationPublisher;
            }
        }
        
        // Connect last IRevocation... to TransportOutput (this class)
        if ( previousRevocationPublisher != nullptr )
        {
            //TODO
            this->contextFilterByState = std::bind( &TransportSubscriber::filterCommittedContexts, this, std::placeholders::_1, std::placeholders::_2 );

            // propagate this->onRevoked into the previousRevocationPublisher->onRevoked
            previousRevocationPublisher->onRevoked += std::bind(
                                                            &Quix::EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::IRevocationPublisher::OnRevokedEventArgs &>::operator(),
                                                            this->onRevoked, 
                                                            std::placeholders::_1, 
                                                            std::placeholders::_2
                                                        );
            
            // propagate this->onRevoking into the previousRevocationPublisher->onRevoking
            previousRevocationPublisher->onRevoking += std::bind(
                                                            &Quix::EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::IRevocationPublisher::OnRevokingEventArgs &>::operator(),
                                                            this->onRevoking, 
                                                            std::placeholders::_1, 
                                                            std::placeholders::_2
                                                        );
        }
        // Done hooking up modifiers implementing IRevocation

    }

    void TransportSubscriber::commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts)
    {
        this->onCommit_(transportContexts);
    }


    void TransportSubscriber::close()
    {
        if( commitModifier_ != nullptr )
        {
            commitModifier_->close();
        }
    }


    TransportSubscriber::~TransportSubscriber()
    {
        if( commitModifier_ != nullptr )
        {
            delete commitModifier_;
        }
    }

    void TransportSubscriber::sendInternal(std::shared_ptr<IPackage> package)
    {
        onNewPackage(package);
    }

} }
// A2DD.cpp
#include "transport.h"
#include "./transportSubscriber.h"
#include "./fw/deserializingModifier.h"
#include "./fw/IModifier.h"
#include "./io/IPackage.h"

#include <functional>
#include <iostream>
#include <vector>


namespace Quix { namespace Transport {

    TransportSubscriber::TransportSubscriber(ISubscriber* subscriber)
    :
    TransportSubscriber( subscriber, [](TransportSubscriberOptions& options){} )
    {

    };

    TransportSubscriber::TransportSubscriber(ISubscriber* subscriber, std::function<void(TransportSubscriberOptions&)> configureOptions)
    :
    byteMergingModifier_(&this->byteMerger_)
    {
 
        // default variables initialization
        onFilterCommittedContexts_ = [](
                void *state, 
                const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter
            ){
                return std::vector<std::shared_ptr<Quix::Transport::TransportContext>>();
            };
        contextFilterByState_ = [](
                void *state, 
                const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter
            ){
                return contextsToFilter;
            };



        TransportSubscriberOptions options;
        configureOptions(options);

        const auto& commitOptions = options.commitOptions;

        std::vector<IModifier*> outputsAndInputs;

        auto firstOutputAndInput = subscriber;

        // output -> merger -> deserializer -> commitModifier -> raise
//        outputsAndInputs.push_back(subscriber);
        outputsAndInputs.push_back(dynamic_cast<IModifier*>(&byteMergingModifier_));
        outputsAndInputs.push_back(dynamic_cast<IModifier*>(&deserializingModifier_));
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
            auto modifierPublisher = dynamic_cast<IPublisher*>(outputsAndInputs[index]);

            auto prevSubscriber = dynamic_cast<ISubscriber*>(previous);
            prevSubscriber->onNewPackage += std::bind( &IPublisher::send, modifierPublisher, std::placeholders::_1 );

            previous = dynamic_cast<ISubscriber*>(outputsAndInputs[index]);
        }

        // Connect last output to TransportSubscriber (this class)
        dynamic_cast<ISubscriber*>(previous)->onNewPackage += std::bind( &TransportSubscriber::sendInternal, this, std::placeholders::_1 );



        // Hook up committing modifiers from front (output) to back (this)
        ICanCommit* previousCanCommitModifier = nullptr;

        //hook up firstOutputAndInput ( subscriber )
        ICanCommit* committingModifier = dynamic_cast<ICanCommit*>( firstOutputAndInput );
        if ( committingModifier != nullptr )
        {
            previousCanCommitModifier = committingModifier;
        }


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
            this->onFilterCommittedContexts_ = std::bind(&ICanCommit::filterCommittedContexts, previousCanCommitModifier, std::placeholders::_1, std::placeholders::_2);
            

            // bound previousCanCommitModifier->onCommitted += this->onCommited(_1, _2)
            previousCanCommitModifier->onCommitted += std::bind(
//                                                            &EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::ICanCommit::OnCommittedEventArgs &>::operator(),
                                                            &EventHandler<Quix::Transport::ICanCommit *, const Quix::Transport::ICanCommit::OnCommittedEventArgs &>::operator(),
                                                            this->onCommitted, 
                                                            std::placeholders::_1, 
                                                            std::placeholders::_2
                                                        );
            // bound previousCanCommitModifier->onCommitting += this->onCommiting(_1, _2)
            previousCanCommitModifier->onCommitting += std::bind(
                                                            // &EventHandler<Quix::Transport::IRevocationPublisher *, const Quix::Transport::ICanCommit::OnCommittingEventArgs &>::operator(),
                                                            &EventHandler<Quix::Transport::ICanCommit *, const Quix::Transport::ICanCommit::OnCommittingEventArgs &>::operator(),
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
            this->contextFilterByState_ = std::bind( &TransportSubscriber::filterCommittedContexts, this, std::placeholders::_1, std::placeholders::_2 );

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

    std::vector<std::shared_ptr<TransportContext>> TransportSubscriber::filterCommittedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter)
    {
        return 
            this->onFilterCommittedContexts_ != nullptr 
                ? 
            this->onFilterCommittedContexts_(state, contextsToFilter)
                :
            std::vector<std::shared_ptr<TransportContext>>()
            ;
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


    std::vector<std::shared_ptr<TransportContext>> TransportSubscriber::filterRevokedContexts(void* state, std::vector<std::shared_ptr<TransportContext>> contexts)
    {
        return contextFilterByState_(state, contexts);
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
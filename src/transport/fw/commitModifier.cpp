#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include <algorithm>

#include "../../exceptions/invalidOperationException.h"

#include "../../utils/extensions.h"


#include "./commitModifier.h"

namespace Quix { namespace Transport {

    using namespace std;


    void CommitModifier::onCommitIntervalCallback()
    {
        this->commitTimer_.change(CallbackTimer::INFINITE, CallbackTimer::INFINITE); // Disable flush timer

        {
            std::lock_guard<std::mutex> guard(commitCheckLock_);        

            std::vector<std::shared_ptr<TransportContext>> toCommit;

            {
                std::lock_guard<std::mutex> guard(contextsReadyToCommitLock_); 
                toCommit = listToVec( this->contextsReadyToCommit_ );
            }

            if ( !toCommit.empty() )
            {

                try
                {
                    commit( toCommit );
                    // logger.LogTrace("Committed {0} contexts due to timer expiring", toCommit.Length);
                    acknowledgeTransportContext( toCommit );
                }
                catch(...)
                {
                    // TODO
                    
                    // logger.LogError(ex, "Failed to commit contexts due to timer expiring.");
                }

            }
        }

        // if (closed) return;
        this->commitTimer_.change( commitOptions_.commitInterval ); // Disable flush timer
    }

    CommitModifier::CommitModifier( const CommitOptions& commitOptions )
    :
    commitOptions_(commitOptions),
    commitTimer_( std::bind(&CommitModifier::onCommitIntervalCallback, this) )
    {
        auto commitEvery = commitEvery_ = commitOptions.commitEvery;
        auto autoCommit = commitOptions.autoCommitEnabled;
        auto commitInterval = commitOptions.commitInterval;

        if( !autoCommit )
        {
            onSend_ = std::bind( &CommitModifier::onSendOnAutocommitDisabled, this, std::placeholders::_1 );
            return;
        }

        if( commitEvery == 1 )
        {
            onSend_ = std::bind( &CommitModifier::onSendOnCommitEvery1, this, std::placeholders::_1 );
            return;
        }

        if ( commitInterval > 0 )
        {
            commitTimer_.setAction(
                                [&](){
                    this->commitTimer_.change(Timer::INFINITE, Timer::INFINITE);   //disable flush timer

                    /// commitCheckLock
                    {
                        std::lock_guard<std::mutex> guard( this->commitCheckLock_ );

                        std::vector<std::shared_ptr<TransportContext>> toCommit;
                        {
                            std::lock_guard<std::mutex> guard( this->contextsReadyToCommitLock_ );
                            toCommit = listToVec( this->contextsReadyToCommit_ );
                        }

                        if( toCommit.size() > 0 )
                        {
                            // logger.LogTrace("Committing {0} contexts due to timer expiring", toCommit.Length);
                            try
                            {
                                this->commit(toCommit);
                                // logger.LogTrace("Committed {0} contexts due to timer expiring", toCommit.Length);
                                acknowledgeTransportContext(toCommit);
                            }
                            // catch (Exception ex)
                            catch(...)
                            {
                                // logger.LogError(ex, "Failed to commit contexts due to timer expiring.");
                            }

                        }

                    }   /// commitCheckLock END


                    if( this->closed_ ) { return; }

                    this->commitTimer_.change(commitInterval, Timer::INFINITE);   //disable flush timer
                }
            );
            commitTimer_.change(commitInterval);


            //TODO
        }

        // onRevoking
        onRevoking_ = std::bind( &CommitModifier::onRevokingInternal, this, std::placeholders::_1, std::placeholders::_2 );

        // onRevoked
        onRevoked_ = std::bind( &CommitModifier::onRevokedInternal, this, std::placeholders::_1, std::placeholders::_2 );

        this->onClose += std::bind( &CommitModifier::onCloseInternal, this );

        if( commitEvery <= 0 )
        {
            onSend_ = std::bind( &CommitModifier::onSendOnCommitEveryLessEq0, this, std::placeholders::_1 );
        }
        else
        {
            onSend_ = std::bind( &CommitModifier::onSendOnCommitEveryGt1, this, std::placeholders::_1 );
        }
    }

    void CommitModifier::subscribe(ICanCommit* committer)
    {
        if (this->committer_ != nullptr)
        {
            throw InvalidOperationException("Already connected to a committer");
        }
        this->committer_ = committer;

        this->subscribeCommittedHandler_    = std::bind( &CommitModifier::subscribeCommittedHandlerInternal, this, std::placeholders::_1, std::placeholders::_2 );
        this->committer_->onCommitted       += this->subscribeCommittedHandler_;

        this->subscribeCommittingHandler_   = std::bind( &CommitModifier::subscribeCommittingHandlerInternal, this, std::placeholders::_1, std::placeholders::_2 );
        this->committer_->onCommitting      += this->subscribeCommittingHandler_;

        this->subscribeOnClose_             = std::bind( &CommitModifier::subscribeOnCloseInternal, this );
        this->onClose                       += this->subscribeOnClose_;
    }

    void CommitModifier::subscribeOnCloseInternal()
    {
        this->committer_->onCommitted -= this->subscribeCommittedHandler_;
        this->committer_->onCommitting -= this->subscribeCommittingHandler_;
    }

    void CommitModifier::subscribeCommittedHandlerInternal( ICanCommit* sender, const OnCommittedEventArgs& args )
    {
        this->onCommitted(sender, args);
    }

    void CommitModifier::subscribeCommittingHandlerInternal( ICanCommit* sender, const OnCommittingEventArgs& args )
    {
        this->onCommitting(sender, args);
    }

    void CommitModifier::onUnsubscribePublisher( IRevocationPublisher* revocationPublisher )
    {
        if( this->onRevoked_ != nullptr )
        {
            revocationPublisher->onRevoked -= this->onRevoked_;
        }
        if( this->onRevoking_ != nullptr )
        {
            revocationPublisher->onRevoking -= this->onRevoking_;
        }
    }

    void CommitModifier::subscribe( IRevocationPublisher* revocationPublisher )
    {
        if ( this->onRevoked_ != nullptr )
        {
            revocationPublisher->onRevoked += this->onRevoked_;
        }

        if ( this->onRevoking_ != nullptr )
        {
            revocationPublisher->onRevoking += this->onRevoking_;
        }

        if ( this->onRevoked_ != nullptr || this->onRevoking_ != nullptr )
        {
            this->onClose += std::bind(&CommitModifier::onUnsubscribePublisher, this, revocationPublisher);
        }

    }    

    void CommitModifier::onSendOnAutocommitDisabled(std::shared_ptr<IPackage> package) const
    {
        if(this->onNewPackage != nullptr)
        {
            this->onNewPackage(package);
        }
    }

    void CommitModifier::onSendOnCommitEvery1(std::shared_ptr<IPackage> package)
    {
        //TODO: cancellationToken

        this->onNewPackage(package);
        if (package.get() == nullptr) return;   //TODO: maybe not neccessary ( remaint from the Csharp rewrite )
        if (this->closed_) return;
//        logger.LogTrace("Committing contexts due to reaching limit {0}", commitEvery);
        this->commitSingle(package->transportContext());
        // logger.LogTrace("Committed contexts due to reaching limit {0}", commitEvery);
    }

    void CommitModifier::onSendOnCommitEveryLessEq0(std::shared_ptr<IPackage> package)
    {
        //TODO: cancellationToken

        this->onNewPackage(package);
        // await (this.OnNewPackage?.Invoke(package) ?? Task.CompletedTask);
        if (package.get() == nullptr) return;   //TODO: maybe not neccessary ( remaint from the Csharp rewrite )
        if (this->closed_) return;

        {
            std::lock_guard<std::mutex> guard(this->contextsReadyToCommitLock_);        
            this->contextsReadyToCommit_.push_back(package->transportContext());
        }
    }

    void CommitModifier::onSendOnCommitEveryGt1(std::shared_ptr<IPackage> package)
    {
        //TODO: cancellationToken

        this->onNewPackage(package);
        if (package.get() == nullptr) { return; }   //TODO: maybe not neccessary ( remaint from the Csharp rewrite )
        if (this->closed_) { return; }

        int contextsReadyToCommitCnt;
        {
            std::lock_guard<std::mutex> guard( this->contextsReadyToCommitLock_ );
            this->contextsReadyToCommit_.push_back( package->transportContext() );

            contextsReadyToCommitCnt = this->contextsReadyToCommit_.size( );
        }

        if( contextsReadyToCommitCnt >= commitEvery_ )
        {
            std::lock_guard<std::mutex> guard( this->commitCheckLock_ );

            int contextsReadyToCommitCntNew = contextsReadyToCommit_.size( );
            if ( contextsReadyToCommitCntNew < contextsReadyToCommitCnt ) { return; } // in case the condition changed after acquiring the lock

            std::vector<std::shared_ptr<TransportContext>> toCommit;
            toCommit = listToVec( this->contextsReadyToCommit_ );

            // logger.LogTrace("Committing contexts due to reaching limit {0}", commitEvery);
            this->commit( toCommit );
            // logger.LogTrace("Committed contexts due to reaching limit {0}", commitEvery);
            this->acknowledgeTransportContext( toCommit );

        }
    }


    void CommitModifier::onRevokedInternal(IRevocationPublisher* sender, const IRevocationPublisher::OnRevokedEventArgs& args)
    {
        std::lock_guard<std::mutex> guard(this->commitCheckLock_);

        std::vector<std::shared_ptr<TransportContext>> contexts;

        {
            std::lock_guard<std::mutex> guard(this->contextsReadyToCommitLock_);
            contexts.reserve(this->contextsReadyToCommit_.size());
            for( auto& el: this->contextsReadyToCommit_ )
            {
                contexts.push_back(el);
            }
        }

        if (contexts.empty() )
        {
            return;
        }


        auto affectedContexts = sender->filterRevokedContexts(args.state(), contexts);

        // Theoretically it is possible to have a concurrency issue here. The issue is that the old queue gets enumerated and creates a new queue.
        // Then it gets assigned to the variable. Between creating the new queue and assigning it to the variable another thread could still add to the
        // old queue which would no longer get evaluated.
        {
            std::list<std::shared_ptr<TransportContext>> filteredContexts;

            for( auto el : contextsReadyToCommit_ )
            {
                
                auto end = affectedContexts.end();

                auto it = find_if( 
                    affectedContexts.begin(), 
                    end, 
                    [&](const std::shared_ptr<TransportContext>& element) 
                    {
                        return element.get() == el.get();
                    } 
                );

                if( it == end )
                {
                    filteredContexts.push_back(el);
                }
            }

            this->contextsReadyToCommit_ = filteredContexts;
        }

        if ( !affectedContexts.empty() )
        {
            // logger.LogWarning("Discarding {0} contexts due to already finished revocation.", contexts.Length);
        }

    }

    void CommitModifier::onRevokingInternal(IRevocationPublisher* sender, const IRevocationPublisher::OnRevokingEventArgs& args)
    {
        std::lock_guard<std::mutex> guard(this->commitCheckLock_);

        std::vector<std::shared_ptr<TransportContext>> contexts;

        {
            std::lock_guard<std::mutex> guard(this->contextsReadyToCommitLock_);
            contexts.reserve(this->contextsReadyToCommit_.size());
            for( auto& el: this->contextsReadyToCommit_ )
            {
                contexts.push_back(el);
            }
        }

        if (contexts.empty() )
        {
            return;
        }

        auto affectedContexts = sender->filterRevokedContexts(args.state(), contexts);

        if( affectedContexts.empty() )
        {
            return;
        }

        // logger.LogDebug("Committing {0} contexts due to revocation, {1} contexts unaffected.", affectedContexts.Length, contexts.Length - affectedContexts.Length);

        try
        {

            this->commit(affectedContexts);

            // logger.LogDebug("Committed {0} contexts due to revocation.", affectedContexts.Length);
                
            // Theoretically it is possible to have a concurrency issue here. The issue is that the old queue gets enumerated and creates a new queue.
            // Then it gets assigned to the variable. Between creating the new queue and assigning it to the variable another thread could still add to the
            // old queue which would no longer get evaluated.
            {
                std::lock_guard<std::mutex> guard(this->contextsReadyToCommitLock_);

                std::list<std::shared_ptr<TransportContext>> filteredContexts;
                for( auto el : contextsReadyToCommit_ )
                {
                    //TODO: this is O(n) so the whole loop is O(n*n), but maybe it does not matter because of common sizes

                    //TODO: this is pointer and not content check

                    auto end = affectedContexts.end();
                    auto it = find_if( affectedContexts.begin(), affectedContexts.end() , [&](const std::shared_ptr<TransportContext>& element) {
                        return element.get() == el.get();
                    } );

                    if( it == end )
                    {
                        filteredContexts.push_back(el);
                    }
                }

                this->contextsReadyToCommit_ = filteredContexts;
            }

        }
        catch (...)
        {
            // logger.LogError(ex, "Failed to commit contexts due to revocation.");
        }
    }


    std::vector<std::shared_ptr<TransportContext>> CommitModifier::filterCommittedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter)
    {
        return this->committer_ != nullptr
                 ? 
                this->committer_->filterCommittedContexts(state, contextsToFilter)
                 :
                std::vector<std::shared_ptr<TransportContext>>(); 
    }
    

    void CommitModifier::commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts)
    {
        if( this->committer_ != nullptr )
        {
            this->committer_->commit(transportContexts);
        }        
    }

    void CommitModifier::close()
    {
        if (closed_) return;
        closed_ = true;
        onClose();

        commitTimer_.change(Timer::INFINITE, Timer::INFINITE);
    }

    void CommitModifier::acknowledgeTransportContext(const std::vector<std::shared_ptr<TransportContext>>& acknowledge)
    {
        std::lock_guard<std::mutex> guard(contextsReadyToCommitLock_);        

        for (auto& transportContext : acknowledge )
        {
            if( contextsReadyToCommit_.empty() ) { 
                // this.logger.LogWarning("Tried to acknowledge more item than there is in the queue.");
                return;
            }
            auto& queueContext = contextsReadyToCommit_.front();

            //TODO: maybe equality check
            if (queueContext.get() != transportContext.get())
            {
                // this.logger.LogWarning("The next item in the transport queue is not what is expected. Avoiding acknowledging item.");
                return;
            }

            // at this point we are certain of what is getting dequeued, so no checking.
            contextsReadyToCommit_.pop_front();
        }

    }


    void CommitModifier::onCloseInternal()
    {
        std::lock_guard<std::mutex> guard(commitCheckLock_);

        std::vector<std::shared_ptr<TransportContext>> toCommit;
        {
            std::lock_guard<std::mutex> guard(contextsReadyToCommitLock_);        

            toCommit.reserve(contextsReadyToCommit_.size());
            for(auto& el : contextsReadyToCommit_)
            {
                toCommit.push_back(el);
            }
        }


        if (toCommit.size() <= 0) return;

        // logger.LogTrace("Committing {0} contexts due to close", toCommit.Length);
        this->commit(toCommit);
        // logger.LogTrace("Committed {0} contexts due to close", toCommit.Length);
        this->acknowledgeTransportContext(toCommit);
    }


    void CommitModifier::send(std::shared_ptr<IPackage> package)
    {
        //TODO: add cancellation token
        onSend_(package);
    }

} }
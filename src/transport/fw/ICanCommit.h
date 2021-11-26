#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to take Package
 */
class ICanCommit{

public:

    class OnCommittedEventArgs{
    private:
        void* state_;
    public:
        inline OnCommittedEventArgs(void* state = nullptr)
        :
        state_(state)
        {

        }

        inline void* state()
        {
            return state_; 
        }

        inline void* setState(void* state = nullptr)
        {
            return state_ = state; 
        }

    };

    class OnCommittingEventArgs{
    private:
        void* state_;
    public:

        inline OnCommittingEventArgs(void* state = nullptr)
        :
        state_(state)
        {

        }

        inline void* state()
        {
            return state_; 
        }
    };

    /**
     * Helper method wrapping ICommitter.Commit
     * 
     * @param transportContext The transport context to commit
     */
    void commit(const TransportContext& transportContext);

    /**
     * Commits the transport context to the output.
     * 
     * @param transportContexts The transport context to commit
     */
    virtual void commit(const std::vector<TransportContext>& transportContexts) = 0;

    /**
     * Event is raised when the transport context finished committing
     */
    EventHandler<const OnCommittedEventArgs&> onCommitted;
        
    /**
     * Event is raised when the transport context starts committing. It is not guaranteed to be raised if underlying broker initiates commit on its own
     */
    EventHandler<const OnCommittingEventArgs&> onCommitting;

};

/**
 * Describes an interface for subscribing to an ICanCommit
 */
class ICanCommitSubscriber{
    virtual void subscribe(ICanCommit* committer) = 0; 
};

} }
#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"

#include "./IRevocation.h"
#include "./IModifier.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Describes an interface for committing 
 */
class ICanCommit{

    /***
     * @brief Event args which state is immutable / frozen
     */
    class ReadonlyEventArgs{
    protected:
        void* state_;
    public:
        inline ReadonlyEventArgs(void* state = nullptr)
        :
        state_(state)
        {

        }

        inline void* state() const
        {
            return state_; 
        }

        bool operator==(const ReadonlyEventArgs& other) const
        {
            return state() == other.state();
        }
    };

    /***
     * @brief Event args which state can be modifier
     */
    class ReadWriteEventArgs : public ReadonlyEventArgs{
    public:
        inline ReadWriteEventArgs(void* state = nullptr) : ReadonlyEventArgs(state) {}
        inline void* setState(void* state = nullptr)
        {
            return state_ = state; 
        }
    };


public:

    /***
     * @brief Event args for ICanCommit.onCommitted
     */
    class OnCommittedEventArgs : public ReadWriteEventArgs {
    public:
        inline OnCommittedEventArgs(void* state = nullptr) : ReadWriteEventArgs(state) {}
    };

    /***
     * @brief Event args for ICanCommit.onCommitting
     */
    class OnCommittingEventArgs : public ReadonlyEventArgs {
    public:
        inline OnCommittingEventArgs(void* state = nullptr) : ReadonlyEventArgs(state) {}
    };

    /**
     * Helper method wrapping ICommitter.Commit
     * 
     * @param transportContext The transport context to commit
     */
    void commitSingle(const std::shared_ptr<TransportContext>& transportContext);

    /**
     * Commits the transport context to the output.
     * 
     * @param transportContexts The transport context to commit
     */
    virtual void commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) = 0;

    /**
     * Event is raised when the transport context finished committing
     */
    EventHandler<ICanCommit*, const OnCommittedEventArgs&> onCommitted;
        
    /**
     * Event is raised when the transport context starts committing. It is not guaranteed to be raised if underlying broker initiates commit on its own
     */
    EventHandler<ICanCommit*, const OnCommittingEventArgs&> onCommitting;

    /**
     * @brief Filters contexts that were affected by the commit.
     * @param state State raised by onCommitted
     * @param contextsToFilter The contexts to filter
     * @return Contexts affected by the commit
     * 
     */
    virtual std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) = 0;

};

/**
 * Describes an interface for subscribing to an ICanCommit
 */
class ICanCommitSubscriber{
public:
    virtual void subscribe(ICanCommit* committer) = 0; 
};

} }
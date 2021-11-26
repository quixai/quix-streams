#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to take Package
 */
class IRevocationPublisher{
    class BaseRevocationArgs{
    private:
        void* state_;
    public:
        inline BaseRevocationArgs(void* state = nullptr)
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

public:

    class OnRevokingEventArgs : public BaseRevocationArgs {
    };

    class OnRevokedEventArgs : public BaseRevocationArgs {
    };


    /**
     * Raised when losing access to source depending on implementation
     * Argument is the state which describes what is being revoked, depending on implementation
     */
    EventHandler<const OnRevokingEventArgs&> onRevoking;

    /**
     * Raised when lost access to source depending on implementation
     * Argument is the state which describes what got revoked, depending on implementation
     */
    EventHandler<const OnRevokedEventArgs&> onRevoked;

    /**
     * Filters contexts affected by the revocation.
     * 
     * @param state State raised as argument of onRevoking or onRevoked
     * @param contexts The contexts to filter
     * 
     * @return Contexts affected by the state
     */
    virtual std::vector<TransportContext> filterRevokedContexts(void* state, std::vector<TransportContext> contexts) = 0;


};

/**
 * Subscribes to a IRevocationPublisher
 */
class IRevocationSubscriber{
    virtual void subscribe(IRevocationPublisher* revocationPublisher) = 0; 
};

} }
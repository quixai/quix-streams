#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"
#include "../../utils/object.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Describes interface for publishing information about revocation
 */
class IRevocationPublisher{
    class BaseRevocationArgs : public Object {
    private:
        Object* state_;
    public:
        inline BaseRevocationArgs(Object* state = nullptr)
        :
        state_(state)
        {

        }

        inline Object* state() const
        {
            return state_; 
        }

        inline Object* setState(Object* state = nullptr)
        {
            return state_ = state; 
        }

        bool operator==( const BaseRevocationArgs& other ) const {
            return state() == other.state();
        }
        
    };

public:

    class OnRevokingEventArgs : public BaseRevocationArgs {
        public:
            OnRevokingEventArgs( Object* state = nullptr ) : BaseRevocationArgs(state) {};
    };

    class OnRevokedEventArgs : public BaseRevocationArgs {
        public:
            OnRevokedEventArgs( Object* state = nullptr ) : BaseRevocationArgs(state) {};
    };


    /**
     * Raised when losing access to source depending on implementation
     * Argument is the state which describes what is being revoked, depending on implementation
     */ 
    EventHandler<IRevocationPublisher*, const OnRevokingEventArgs&> onRevoking;

    /**
     * Raised when lost access to source depending on implementation
     * Argument is the state which describes what got revoked, depending on implementation
     */
    EventHandler<IRevocationPublisher*, const OnRevokedEventArgs&> onRevoked;

    /**
     * Filters contexts affected by the revocation.
     * 
     * @param state State raised as argument of onRevoking or onRevoked
     * @param contexts The contexts to filter
     * 
     * @return Contexts affected by the state
     */
    virtual std::vector<std::shared_ptr<TransportContext>> filterRevokedContexts(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contexts) = 0;


};

/**
 * Subscribes to a IRevocationPublisher
 */
class IRevocationSubscriber{
public:
    virtual void subscribe(IRevocationPublisher* revocationPublisher) = 0; 
};

} }
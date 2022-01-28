#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>

#include <list>

#include <mutex>

#include "../../utils/timer.h"
#include "../../utils/eventHandler.h"

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "../io/ISubscriber.h"
#include "./IRevocation.h"
#include "./ICanCommit.h"

#include "./IModifier.h"

namespace Quix { namespace Transport {

class CommitOptions
{
public:
    /**
     * @brief Gets or sets whether automatic committing is enabled.
     * If automatic committing is not enabled, other values are ignored.
     */
    bool autoCommitEnabled = true;
    /**
     * @brief The interval of automatic commit in ms
     * negative means same as null in C#
     */
    int commitInterval = 5000;
    /**
     * @brief The number of messages to automatically commit at
     */
    int commitEvery = 5000;
};

/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class CommitModifier : public IModifier, public ICanCommit, public ICanCommitSubscriber, public IRevocationSubscriber /*: public ISubscriber, ICanCommit, ICanCommitSubscriber, IRevocationSubscriber */
{
private:

    bool closed_ = false;

    /**
     * @brief The number of messages to automatically commit at
     */
    int commitEvery_;

    EventHandler<> onClose;

    ICanCommit* committer_ = nullptr;
    std::function<void(std::shared_ptr<IPackage>)> onSend_ = nullptr;

    std::function<void(IRevocationPublisher*, const IRevocationPublisher::OnRevokingEventArgs&)> onRevoking_;
    std::function<void(IRevocationPublisher*, const IRevocationPublisher::OnRevokedEventArgs&)>  onRevoked_;


    std::list<std::shared_ptr<TransportContext>> contextsReadyToCommit_;
    std::mutex contextsReadyToCommitLock_;
    // this is used to avoid two concurrent commits at the same time. The commits could possibly be a problem
    // if they happened out of order
    std::mutex commitCheckLock_;

    CallbackTimer commitTimer_;

    const CommitOptions commitOptions_;


    void acknowledgeTransportContext(const std::vector<std::shared_ptr<TransportContext>>& acknowledge);

    /**
     *  On send callback
     *  In case there is no auto committing then all we have to do is pass the message up in the chain
     */
    void onSendOnAutocommitDisabled(std::shared_ptr<IPackage> package) const;
    /**
     *  On send callback
     *  if we're committing every single message, then any kind of timer based commit is irrelevant
     */
    void onSendOnCommitEvery1(std::shared_ptr<IPackage> package);

    /**
     *  On send callback
     *  commiting every n message is disabled and we queue everything and rely on timer to commit
     */
    void onSendOnCommitEveryLessEq0(std::shared_ptr<IPackage> package);
    /**
     *  On send callback
     *  commiting every n message is enabled
     */
    void onSendOnCommitEveryGt1(std::shared_ptr<IPackage> package);

    /// On revoking callback
    void onRevokedInternal(IRevocationPublisher*, const IRevocationPublisher::OnRevokedEventArgs&);
    /// On revoked callback
    void onRevokingInternal(IRevocationPublisher*, const IRevocationPublisher::OnRevokingEventArgs&);

    /// On close callback
    void onCloseInternal();

    /**
     *  Function handling committing by internval
     */
    void onCommitIntervalCallback();

    /**
     * Function handling the onClose for the ICanCommit* subscriber  
     * @param  ICanCommit subscriber source of the event
     */
    void subscribeOnCloseInternal();
    /// stores subscribeOnCloseInternal function binded with this as first argument
    EventHandlerFunction<> subscribeOnClose_;

    /**
     * Function handling the onCommited for the ICanCommit* subscriber  
     * @param subscriber source of the event
     * @param args passed arguments
     */
    void subscribeCommittedHandlerInternal(ICanCommit* subscriber, const OnCommittedEventArgs& args);
    /// stores subscribeCommittedHandlerInternal function binded with this as first argument
    EventHandlerFunction<ICanCommit*, const OnCommittedEventArgs&> subscribeCommittedHandler_;

    /**
     * Function handling the onCommitting for the ICanCommit* subscriber  
     * @param  ICanCommit subscriber source of the event
     * @param args passed arguments
     */
    void subscribeCommittingHandlerInternal(ICanCommit* subscriber, const OnCommittingEventArgs& args);
    /// stores subscribeCommittingHandlerInternal function binded with this as first argument
    EventHandlerFunction<ICanCommit*, const OnCommittingEventArgs&> subscribeCommittingHandler_;

    void onUnsubscribePublisher(IRevocationPublisher* revocationPublisher);

public:

    /***
     * @brief Commit modifier which enables committing packages in an automatic fashion
     */
    CommitModifier(const CommitOptions& commitOptions);

    void commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts);

    /**
     * Subscriber to committer  
     * @param commiter to be subscribed to
     */
    void subscribe(ICanCommit* committer);
    /**
     * Subscriber to revocation publisher  
     * @param revocationPublisher to be subscribed to
     */
    void subscribe(IRevocationPublisher* revocationPublisher);

    void close();

    /**
     * Send a package, which the modifier queues untill committed. Commited results are raised via onNewPackage
     * 
     * @param package The package to be sent
     */
    void send(std::shared_ptr<IPackage> package);

    std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter);


};

} }



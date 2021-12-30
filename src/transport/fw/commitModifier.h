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
class CommitModifier : public ICanCommit, IPublisher, public ICanCommitSubscriber, public IRevocationSubscriber /*: public ISubscriber, ICanCommit, ICanCommitSubscriber, IRevocationSubscriber */
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

    /// In case there is no auto committing then all we have to do is pass the message up in the chain
    void onSendOnAutocommitDisabled(std::shared_ptr<IPackage> package) const;
    /// if we're committing every single message, then any kind of timer based commit is irrelevant
    void onSendOnCommitEvery1(std::shared_ptr<IPackage> package);

    void onSendOnCommitEveryLessEq0(std::shared_ptr<IPackage> package);
    void onSendOnCommitEveryGt1(std::shared_ptr<IPackage> package);

    void onRevokedInternal(IRevocationPublisher*, const IRevocationPublisher::OnRevokedEventArgs&);
    void onRevokingInternal(IRevocationPublisher*, const IRevocationPublisher::OnRevokingEventArgs&);

    void onCommitIntervalCallback();

    void onCloseInternal();

    void subscribeOnCloseInternal();
    std::function<void()> subscribeOnClose_ = nullptr;

    void subscribeCommittedHandlerInternal(ICanCommit*, const OnCommittedEventArgs&);
    std::function<void(ICanCommit*, const OnCommittedEventArgs&)> subscribeCommittedHandler_ = nullptr;

    void subscribeCommittingHandlerInternal(ICanCommit*, const OnCommittingEventArgs&);
    std::function<void(ICanCommit*, const OnCommittingEventArgs&)> subscribeCommittingHandler_ = nullptr;

    void onUnsubscribePublisher(IRevocationPublisher* revocationPublisher);

public:

    //TODO: remove
    std::function<void(std::shared_ptr<IPackage>)> onNewPackage = nullptr;

    CommitModifier(const CommitOptions& commitOptions);

    void commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts);

    void subscribe(ICanCommit* committer);
    void subscribe(IRevocationPublisher* revocationPublisher);


    void close();

    void send(std::shared_ptr<IPackage> package);

    std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter);


};

} }



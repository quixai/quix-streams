#pragma once

#include <functional>

#include "./fw/deserializingModifier.h"
#include "./fw/byteMergingModifier.h"
#include "./fw/byteMerger.h"
#include "./fw/commitModifier.h"
#include "./fw/ICanCommit.h"
#include "./io/IPackage.h"
#include "./io/ISubscriber.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

class TransportSubscriberOptions
{
  public:
    CommitOptions commitOptions;
};

/**
 * A prebuilt pipeline, which deserializes and merges the package's output by the specified ISubscriber
 */
class TransportSubscriber : public ISubscriber, public ICanCommit, public IRevocationPublisher
{

private:
  DeserializingModifier deserializingModifier_;
  ByteMerger            byteMerger_;
  ByteMergingModifier   byteMergingModifier_;
  CommitModifier*       commitModifier_;

  std::function<void( const std::vector<std::shared_ptr<TransportContext>>& acknowledge )> onCommit_;

  void onCommitClass( ICanCommit* previousCanCommitModifier, std::vector<std::shared_ptr<TransportContext>>& acknowledge );


  std::function<std::vector<std::shared_ptr<Quix::Transport::TransportContext>>( void *state, const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter )> onFilterCommittedContexts_;
  std::function<std::vector<std::shared_ptr<Quix::Transport::TransportContext>>( void *state, const std::vector<std::shared_ptr<Quix::Transport::TransportContext>> &contextsToFilter )> contextFilterByState_;

  /**
   * Internal function to handle the package from end of pipeline
   */
  void sendInternal( std::shared_ptr<IPackage> package );

public:
  /**
   * Initializes a new instance of TransportSubscriber, which listens to the specified ISubscriber
   * 
   * @param subscriber The subscriber to listen to
   */
  TransportSubscriber( ISubscriber* subscriber );
  /**
   * Initializes a new instance of TransportSubscriber, which listens to the specified ISubscriber
   * 
   * @param subscriber The subscriber to listen to
   * @param configureOptions Callback to modify the options object
   */
  TransportSubscriber( ISubscriber* subscriber, std::function<void(TransportSubscriberOptions&)> configureOptions );

  std::vector<std::shared_ptr<TransportContext>> filterRevokedContexts  ( void* state, std::vector<std::shared_ptr<TransportContext>> contexts );  
  std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts( void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter );

  void onRevokingInternal( Quix::Transport::IRevocationPublisher *,  const Quix::Transport::IRevocationPublisher::OnRevokingEventArgs & );
  void onRevokedInternal ( Quix::Transport::IRevocationPublisher *,   const Quix::Transport::IRevocationPublisher::OnRevokedEventArgs & );

  void onCommittingInternal( Quix::Transport::ICanCommit *, const Quix::Transport::ICanCommit::OnCommittingEventArgs &  );
  void onCommittedInternal ( Quix::Transport::ICanCommit *, const Quix::Transport::ICanCommit::OnCommittedEventArgs &   );

  void close( );

  void commit( const std::vector<std::shared_ptr<TransportContext>>& transportContexts );

  ~TransportSubscriber();

};

} }

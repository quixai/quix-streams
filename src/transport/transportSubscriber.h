#pragma once

#include "./fw/deserializingModifier.h"
#include "./io/IPackage.h"
#include "./io/ISubscriber.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

/**
 * A prebuilt pipeline, which deserializes and merges the package's output by the specified ISubscriber
 */
class TransportSubscriber : public ISubscriber
{

private:
  DeserializingModifier deserializingModifier_;

  /**
   * Internal function to handle the package from end of pipeline
   */
  void sendInternal(std::shared_ptr<IPackage> package);

public:
  /**
   * Initializes a new instance of TransportSubscriber, which listens to the specified ISubscriber
   * 
   * @param subscriber The subscriber to listen to
   */
  TransportSubscriber(ISubscriber* subscriber);

};

} }

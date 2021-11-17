#pragma once

#include <memory>

#include "./fw/byteSplittingModifier.h"
#include "./fw/serializingModifier.h"
#include "./io/IPackage.h"
#include "./io/IPublisher.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

/**
 * A prebuilt pipeline, which serializes and optionally splits the provided packages then passes into the specified input.
 */
class TransportPublisher : public IPublisher
{

private:
  SerializingModifier serializingModifier_;
  ByteSplittingModifier byteSplittingModifier_;

public:
  /**
   * Initializes a new instance of TransportPublisher with the specified ISubscriber
   *
   * @param publisher The publisher to pass the serialized packages into
   * @param byteSplitter The optional byte splitter to use
   */
  TransportPublisher(IPublisher* input, ByteSplitter* byteSplitter = nullptr);

  /**
   * Send a package, which the TransportPublisher serializes and optionally splits then passes to the provided IPublisher
   */
  void send(std::shared_ptr<IPackage> package) const;
};

} }

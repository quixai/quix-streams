#pragma once

#include <memory>

#include "./fw/serializingModifier.h"
#include "./io/IPackage.h"
#include "./io/IPublisher.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

class TransportPublisher : public ISubscriber
{

private:
  SerializingModifier serializer;

public:
  TransportPublisher(IPublisher* input, ByteSplitter* byteSplitter = nullptr);
  void send(std::shared_ptr<IPackage> package);
};

} }

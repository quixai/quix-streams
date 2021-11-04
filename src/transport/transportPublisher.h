#pragma once

#include "./fw/serializingModifier.h"
#include "./io/IPackage.h"
#include "./io/IPublisher.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

class TransportPublisher
{

private:
  SerializingModifier serializer;

public:
  TransportPublisher(IPublisher* input, ByteSplitter* byteSplitter = nullptr);
  void send(IPackage* package);
};

} }

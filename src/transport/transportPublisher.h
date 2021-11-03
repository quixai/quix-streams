#pragma once

#include "./fw/serializingModifier.h"
#include "./io/abstractPackage.h"
#include "./io/abstractInput.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

class TransportPublisher
{

private:
  SerializingModifier serializer;

public:
  TransportPublisher(AbstractInput* input, ByteSplitter* byteSplitter = nullptr);
  void send(AbstractPackage* package);
};

} }

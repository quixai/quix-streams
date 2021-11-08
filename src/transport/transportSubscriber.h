#pragma once

#include "./fw/deserializingModifier.h"
#include "./io/IPackage.h"
#include "./io/ISubscriber.h"
#include "./fw/byteSplitter.h"


namespace Quix { namespace Transport {

class TransportSubscriber : public ISubscriber
{

private:
  DeserializingModifier deserializingModifier_;

  void sendInternal(std::shared_ptr<IPackage> package);

public:
  TransportSubscriber(ISubscriber* output);

};

} }

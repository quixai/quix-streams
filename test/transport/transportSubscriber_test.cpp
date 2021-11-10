#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/io/Package.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "./helpers/testModel.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

///TODO: Commiting & Revoking tests
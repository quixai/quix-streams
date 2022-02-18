
#include "./readMessage.h"

#include "transport/registry/codecRegistry.h"

#include <iostream>

using namespace std;
using namespace Quix::Transport;


const Quix::Transport::BinaryCodec<ExampleModel> ReadMessage::codec1 = Quix::Transport::BinaryCodec<ExampleModel>();

void ReadMessage::start()
{
    cout << "START READ MESSAGE" << endl;
}

void ReadMessage::registerCodecs()
{
    auto registry = CodecRegistry::instance();
    registry->registerCodec( ModelKey::forType<ExampleModel>() , &codec1 );

}

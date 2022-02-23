
#include "./constants.h"

#include <functional>
#include <sstream>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

shared_ptr<IPackage> Quix::Transport::Kafka::Constants::keepAlivePackage( new Quix::Transport::Package<ByteArray>( ByteArray() ) );
regex Quix::Transport::Kafka::Constants::exceptionMsRegex( " (\\d+)ms" );


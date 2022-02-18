#include "./samples/readMessage.h"
#include "./samples/writeMessage.h"

#include <ctime>       /* time */
#include <cstdlib>     /* srand, rand */



void exampleReadWriteMessage()
{
    ReadMessage readMessage;
    WriteMessage writeMessage;

    readMessage.start();
    writeMessage.start();

    readMessage.join();
    writeMessage.join();
}


int main(int argc, char **argv)
{
    srand (time(NULL));

    exampleReadWriteMessage();

    return 0;
}
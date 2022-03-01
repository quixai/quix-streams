#include "./samples/readMessage.h"
#include "./samples/writeMessage.h"

#include <ctime>       /* time */
#if _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#include <cstdlib>     /* srand, rand */



void exampleReadWriteMessage()
{
    ReadMessage readMessage;
    WriteMessage writeMessage;

    readMessage.start();
    writeMessage.start();

    writeMessage.join();
}


int main(int argc, char **argv)
{
    srand (time(NULL));

    exampleReadWriteMessage();

    return 0;
}
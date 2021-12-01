#include "gtest/gtest.h"
#include "utils/eventHandler.h"

#include <algorithm>
#include <mutex>

using namespace std;
using namespace Quix::Transport;


class MyException : public std::exception { };


std::mutex execMutex;
int cnt1;
int cnt2;
void fun()
{
}
void fun1()
{
    cnt1++;
}
void fun2()
{
    cnt2++;
}


TEST(eventHandlerTest, Basic)
{
    /// just to make test thread safe because all tests are using the same variable

    std::lock_guard<std::mutex> guard(execMutex);

    // Arrange
    EventHandler<> eventHandler;

    // Act
    cnt1 = 0;
    cnt2 = 0;

    eventHandler += fun1;
    eventHandler += fun2;

    eventHandler();

    // Assert
    EXPECT_EQ( cnt1, 1);
    EXPECT_EQ( cnt2, 1);
}

TEST(eventHandlerTest, RemoveUnbound)
{
    // Arrange
    EventHandler<> eventHandler;

    // Act & Assert
    EXPECT_NO_THROW( { eventHandler -= fun; } );
}


TEST(eventHandlerTest, TestRemove)
{
    /// just to make test thread safe because all tests are using the same variable
    std::lock_guard<std::mutex> guard(execMutex);

    cnt1 = 0;
    cnt2 = 0;

    // Arrange
    EventHandler<> eventHandler;

    // Act & Assert
    EXPECT_EQ( cnt1, 0 );
    EXPECT_EQ( cnt2, 0 );

    
    eventHandler += fun1; //register handler
    eventHandler();
    EXPECT_EQ( cnt1, 1 );
    EXPECT_EQ( cnt2, 0 );

    eventHandler -= fun1; //unregister handler
    eventHandler(); //nothing should happen

    EXPECT_EQ( cnt1, 1 );
    EXPECT_EQ( cnt2, 0 );
}

TEST(eventHandlerTest, TestRemove2)
{
    /// just to make test thread safe because all tests are using the same variable
    std::lock_guard<std::mutex> guard(execMutex);

    cnt1 = 0;
    cnt2 = 0;

    // Arrange
    EventHandler<> eventHandler;

    // Act & Assert
    EXPECT_EQ( cnt1, 0 );
    EXPECT_EQ( cnt2, 0 );

    eventHandler += fun1;

    eventHandler();
    //increment only 1
    EXPECT_EQ( cnt1, 1 );
    EXPECT_EQ( cnt2, 0 );

    eventHandler += fun2;
    eventHandler();
    //increment both 1 and 2
    EXPECT_EQ( cnt1, 2 );
    EXPECT_EQ( cnt2, 1 );

    //increment only 2
    eventHandler -= fun1;
    eventHandler();
    EXPECT_EQ( cnt1, 2 );
    EXPECT_EQ( cnt2, 2 );
}


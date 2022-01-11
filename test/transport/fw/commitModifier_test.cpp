#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "transport/fw/byteSplitter.h"
#include "transport/fw/commitModifier.h"
#include "transport/fw/ICanCommit.h"
#include "transport/io/IPackage.h"
#include "transport/io/IModel.h"
#include "transport/io/transportContext.h"
#include "transport/fw/exceptions/serializingException.h"

#include <climits>

#include <algorithm>
#include <memory>

using ::testing::_;

using namespace std;
using namespace Quix::Transport;


class TestSingleMessageCommit : public IModel{
    static const size_t MSG_SIZE = 100;
public:
    char msg[MSG_SIZE];

    TestSingleMessageCommit( const char* msg = "" ) { 
        strncpy(this->msg, msg, MSG_SIZE - 1); 
        this->msg[MSG_SIZE - 1] = '\0';
    };

    bool operator==(const TestSingleMessageCommit& other) const {
        return strcmp(msg, other.msg) == 0;
    };

    const ModelKey modelKey() const {
        return Quix::Transport::ModelKey::forType<TestSingleMessageCommit>();
    };

};


class MyMockCommit : public ICanCommit {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterCommittedContexts ,    std::vector<std::shared_ptr<TransportContext>>(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) );

};


void Send_CommitEvery_ShouldCommitAfterAmountReachedInternal(int ackEvery)
{
    // Arrange
    CommitOptions commitOptions;
    commitOptions.autoCommitEnabled = true;
    commitOptions.commitEvery = ackEvery;
    commitOptions.commitInterval = -1;
    
    CommitModifier modifier(commitOptions);
    std::vector<std::vector<std::shared_ptr<TransportContext>>> commits;
    MyMockCommit committer;

    EXPECT_CALL( committer, commit(_) ).Times(3);
    ON_CALL( committer, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            commits.push_back(transportContexts);
        })
    );

    modifier.subscribe(&committer);

    // Act
    for (int i = 0; i < ackEvery * 3; i++)
    {
        std::shared_ptr<IPackage> package(new Package<TestSingleMessageCommit>(TestSingleMessageCommit()));
        modifier.send(package);
    }
    
    // Assert
    EXPECT_EQ( commits.size(), 3 );
    for( auto& el : commits ) {
        EXPECT_EQ( el.size(), ackEvery );
    }
}


TEST(commitModifier, Send_CommitEvery_ShouldCommitAfterAmountReached1) 
{
    Send_CommitEvery_ShouldCommitAfterAmountReachedInternal(1);
}

TEST(commitModifier, Send_CommitEvery_ShouldCommitAfterAmountReached10) 
{
    Send_CommitEvery_ShouldCommitAfterAmountReachedInternal(10);
}




TEST(commitModifierTeest, Send_CommitInterval_ShouldCommitAfterTimeElapsed) 
{
    // Arrange

    CommitOptions commitOptions;
    commitOptions.autoCommitEnabled = true;
    commitOptions.commitEvery       = -1;
    commitOptions.commitInterval    = 2000;

    CommitModifier modifier(commitOptions);
    std::vector<std::chrono::time_point<std::chrono::system_clock>> commitTimes;
    MyMockCommit committer;
    
    std::condition_variable cv;
    std::mutex cv_m;
    bool finished = false;

    ON_CALL( committer, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            std::cout << "COMMIT EXECUTED" << endl;
            commitTimes.push_back(std::chrono::system_clock::now());
            {
                std::unique_lock<std::mutex> lk(cv_m);
                finished = true;
            }
            cv.notify_all();
        })
    );

    modifier.subscribe(&committer);

    // Act

    auto start = std::chrono::system_clock::now();
    std::shared_ptr<IPackage> package(new Package<TestSingleMessageCommit>(TestSingleMessageCommit()));
    modifier.send(package);


    {
        std::unique_lock<std::mutex> lk(cv_m);
        cv.wait(lk, [&]{return finished;});
    }

    // Assert
    EXPECT_EQ( commitTimes.size(), 1 );
    auto timeItTook = commitTimes[0] - start;

    // build agent doesn't can be a bit wonky, so anywhere between 1.8-3 works
    EXPECT_LT( timeItTook, std::chrono::seconds(3) );
    EXPECT_GT( timeItTook, std::chrono::milliseconds(1800) ); // depending on some things it might be slightly less than 2 secs
}


TEST(commitModifier, Send_AutoCommitDisabled_ShouldNotCommit) 
{
    // Arrange

    CommitOptions commitOptions;
    commitOptions.autoCommitEnabled = false;
    commitOptions.commitEvery       = 1;
    commitOptions.commitInterval    = -1;

    CommitModifier modifier(commitOptions);
    std::vector<std::vector<std::shared_ptr<TransportContext>>> commits;
    MyMockCommit committer;

    ON_CALL( committer, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            commits.push_back(transportContexts);
        })
    );

    modifier.subscribe(&committer);


    // Act
    for (int i = 0; i < 12; i++)
    {
        std::shared_ptr<IPackage> package(new Package<TestSingleMessageCommit>(TestSingleMessageCommit()));
        modifier.send(package);
    }
    
    // Assert
    EXPECT_EQ( commits.size(), 0 );
}

TEST(commitModifier, Commit_WithTransport_ShouldRaiseEvent) 
{

    // Arrange

    CommitOptions commitOptions;
    commitOptions.autoCommitEnabled = false;

    CommitModifier modifier(commitOptions);
    std::vector<std::vector<std::shared_ptr<TransportContext>>> commits;
    MyMockCommit committer;

    ON_CALL( committer, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            commits.push_back(transportContexts);
        })
    );

    modifier.subscribe(&committer);

    TransportContext* transportContext = new TransportContext();
    (*transportContext)["abcde"] = "1";

    TransportContext* transportContext2 = new TransportContext();
    (*transportContext2)["abcde"] = "2";

    std::vector<std::shared_ptr<TransportContext>> tocommit;
    tocommit.push_back(std::shared_ptr<TransportContext>(transportContext));
    tocommit.push_back(std::shared_ptr<TransportContext>(transportContext2));

    // Act
    modifier.commit(tocommit);

    // Assert
    EXPECT_EQ( commits.size(), 1 );
    EXPECT_EQ( commits[0].size(), tocommit.size() );
    for( int i = 0; i < tocommit.size(); ++i ) {
        EXPECT_EQ( *(commits[0][i].get()), *(tocommit[i].get()) );
    }

}

TEST(commitModifier, OnNewPackage_ModifierCloses_ShouldCommitOnlyCompleted) 
{
    // Arrange

    // CommitOptions commitOptions;
    // commitOptions.autoCommitEnabled = true;
    // commitOptions.commitEvery       = -1;
    // commitOptions.commitInterval    = -1;

    // CommitModifier modifier(commitOptions);
    // std::vector<std::vector<std::shared_ptr<TransportContext>>> commits;
    // MyMockCommit committer;

    // ON_CALL( committer, commit(_) ).WillByDefault(
    //     testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
    //         commits.push_back(transportContexts);
    //     })
    // );

    // modifier.subscribe(&committer);


    // modifier.onNewPackage += [&](std::shared_ptr<IPackage> package){
    //     packagesReceived.push_back( package );
    // };

}
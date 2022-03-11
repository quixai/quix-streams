#include "gtest/gtest.h"
#include "transport/fw/byteSplitter.h"
#include "transport/fw/byteMerger.h"
#include "transport/io/IPackage.h"
#include "transport/fw/exceptions/serializingException.h"
#include "transport/fw/byteSplitProtocolHeader.h"

#include <climits>

#include <algorithm>
#include <memory>

using namespace std;
using namespace Quix::Transport;

void getSplitData(vector<shared_ptr<ByteArrayPackage>>& splitData, shared_ptr<ByteArrayPackage>& originalData, size_t splitterLen = 50, size_t len = 0)
{
    ByteSplitter splitter(splitterLen);
    auto length = len != 0 ? len : splitter.absoluteMaxMessageSize() - 10 ; // just a bit less than max;

    originalData = shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(ByteArray::initRandom(length))
                    );

    splitData = splitter.split(originalData);
}

void getSplitDataFullPackets(vector<shared_ptr<ByteArrayPackage>>& splitData, shared_ptr<ByteArrayPackage>& originalData, int packets)
{
    ByteSplitter splitter(50);
    auto length = (50 - ByteSplitProtocolHeader::size()) * packets; // just a bit less than max;

    originalData = shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(ByteArray::initRandom(length))
                    );

    splitData = splitter.split(originalData);
}

TEST(byteMergerTest, Merge_WithDataThatIsNotSplit_ShouldReturnSameBytes) 
{
    // Arrange
    uint8_t rawdata[17]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    ByteArray packet = ByteArray::initFromArray(rawdata, 17);
    shared_ptr<ByteArrayPackage> inpPackage(
            new ByteArrayPackage(packet)
        );


    // Act
    ByteMerger merger;  
    shared_ptr<ByteArrayPackage> outPackage(nullptr);

    merger.tryMerge(
        inpPackage,
        string(""),
        outPackage
    );

    // Assert
    EXPECT_NE( outPackage.get(), nullptr );
    EXPECT_EQ( outPackage->value(), inpPackage->value() );
}

TEST(byteMergerTest, Merge_WithSplitDataNotLastMessageFullPackets_ShouldReturnNull) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    getSplitDataFullPackets(splitData, originalData, 2);

    // Act & Assert
    for( size_t index = 0; index < splitData.size() - 1; ++index )
    {
        if( index >= splitData.size() - 1 )
        {
            // last message is tested elsewhere
            break;
        }
        auto& segment = splitData[index];

        shared_ptr<ByteArrayPackage> outPackage(nullptr);
        auto result = merger.tryMerge(segment, string(""), outPackage);
        ASSERT_FALSE( result );
        ASSERT_EQ( outPackage.get(), nullptr );
    }

}

TEST(byteMergerTest, Merge_WithSplitDataNotLastMessage_VariousSize) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    for( int totalLen = 17; totalLen < 200; totalLen++) {
        getSplitData(splitData, originalData, 15, totalLen);

        // Act & Assert
        for( size_t index = 0; index < splitData.size() - 1; ++index )
        {
            if( index >= splitData.size() - 1 )
            {
                // last message is tested elsewhere
                break;
            }
            auto& segment = splitData[index];

            shared_ptr<ByteArrayPackage> outPackage(nullptr);
            auto result = merger.tryMerge(segment, string(""), outPackage);
            ASSERT_FALSE( result );
            ASSERT_EQ( outPackage.get(), nullptr );
        }

        shared_ptr<ByteArrayPackage> outPackage(nullptr);
        auto result = merger.tryMerge(splitData[splitData.size() - 1], string(""), outPackage);
        ASSERT_TRUE( result );
        ASSERT_EQ( outPackage->value(), originalData->value() );        
    }

}

TEST(byteMergerTest, Merge_WithSplitDataNotLastMessage_ShouldReturnNull) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    getSplitData(splitData, originalData);

    // Act & Assert
    for( size_t index = 0; index < splitData.size() - 1; ++index )
    {
        if( index >= splitData.size() - 1 )
        {
            // last message is tested elsewhere
            break;
        }
        auto& segment = splitData[index];

        shared_ptr<ByteArrayPackage> outPackage(nullptr);
        auto result = merger.tryMerge(segment, string(""), outPackage);
        ASSERT_FALSE( result );
        ASSERT_EQ( outPackage.get(), nullptr );
    }

}

TEST(byteMergerTest, Merge_WithSplitDataThatIsLastMessageWithoutRest_ShouldReturnNull) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    getSplitData(splitData, originalData);

    // Act
    shared_ptr<ByteArrayPackage> outPackage(nullptr);
    auto result = merger.tryMerge(splitData[splitData.size()-1], string(""), outPackage);

    // Assert
    ASSERT_FALSE( result );
    ASSERT_EQ( outPackage.get(), nullptr );
}

TEST(byteMergerTest, Merge_WithSplitDataThatIsLastMessageWithRest_ShouldReturnMerged) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    getSplitData(splitData, originalData);

    // Act
    for( size_t index = 0; index < splitData.size() - 1; ++index )
    {
        auto& segment = splitData[index];
        shared_ptr<ByteArrayPackage> outPackage(nullptr);
        auto result = merger.tryMerge(segment, string(""), outPackage);
    }

    shared_ptr<ByteArrayPackage> outPackage(nullptr);
    auto result = merger.tryMerge(splitData[splitData.size() - 1], string(""), outPackage);

    // Assert
    ASSERT_TRUE( result );
    EXPECT_EQ( outPackage->value(), originalData->value() );
}

TEST(byteMergerTest, Purge_WithValidBufferId_ShouldPurgeAllSegments) 
{
    // Arrange
    ByteMerger merger;  
    vector<shared_ptr<ByteArrayPackage>> splitData;
    shared_ptr<ByteArrayPackage> originalData( nullptr );

    getSplitData(splitData, originalData);
    ByteSplitProtocolHeader header(splitData[0]);
    const auto msgId = header.msgId();

    const string msgGroupKey("someRandomKey");

    for (size_t index = 0; index < splitData.size() - 2; index++)
    {
        auto& segment = splitData[index];
        shared_ptr<ByteArrayPackage> outPackage(nullptr);
        merger.tryMerge(segment, msgGroupKey, outPackage);
    }
    
    shared_ptr<ByteArrayPackage> outPackage(nullptr);
    ByteMerger::ByteMergerBufferKey bufferKey;
    merger.tryMerge(splitData[splitData.size() - 2], msgGroupKey, outPackage, bufferKey);

    ASSERT_TRUE( merger.exists(bufferKey, msgId) );

    // Act
    merger.purge(bufferKey);

    // Arrange
    ASSERT_FALSE( merger.exists(bufferKey) );
}

struct MsgPiecesData{
    vector<shared_ptr<ByteArrayPackage>> segments;
    shared_ptr<ByteArrayPackage> data;
};

TEST(byteMergerTest, Merge_InterlacingMessages_ShouldReturnMerged) 
{
    // Arrange
    ByteMerger merger;  
    int uniqueMessageCount = 10;

    MsgPiecesData splitData[10];
    for( int i = 0; i < uniqueMessageCount; ++i ){
        getSplitData( splitData[i].segments, splitData[i].data );
    }
            
    int splitCount = splitData[0].segments.size();

    // Act
    for (int segmentIndex = 0; segmentIndex < splitCount - 1; segmentIndex++) // for each msg send one segment, then repeat till last -1
    {
        for (int msgIndex = 0; msgIndex < uniqueMessageCount; msgIndex++)
        {
            auto& segment = splitData[ msgIndex ].segments[ segmentIndex ];
            shared_ptr<ByteArrayPackage> outPackage( nullptr );
            merger.tryMerge(segment, string(""), outPackage);
        }
    }
    // Assert
    for (int msgIndex = 0; msgIndex < uniqueMessageCount; msgIndex++)
    {
        auto& sD = splitData[ msgIndex ];
        shared_ptr<ByteArrayPackage> result( nullptr );
        auto success = merger.tryMerge( sD.segments[ sD.segments.size() - 1 ], string(""), result );
        ASSERT_TRUE( success );
        ASSERT_NE( result.get(), nullptr );
        ASSERT_EQ( result->value(), sD.data->value() );
    }
}

TEST(byteMergerTest, Merge_DifferentMessageGroupKey_ShouldReturnMerged) 
{
    // Arrange
    ByteMerger merger;  

    vector<shared_ptr<ByteArrayPackage>> splitData1;
    shared_ptr<ByteArrayPackage> originalData1( nullptr );
    getSplitData(splitData1, originalData1);

    vector<shared_ptr<ByteArrayPackage>> splitData2;
    shared_ptr<ByteArrayPackage> originalData2( nullptr );
    getSplitData(splitData2, originalData2);

    // Act
    for (int index = 0; index < splitData1.size() - 1; index++)
    {
        auto& segment = splitData1[index];
        shared_ptr<ByteArrayPackage> outPackage( nullptr );
        merger.tryMerge(segment, string("Group 1"), outPackage);
    }
    for (int index = 0; index < splitData2.size() - 1; index++)
    {
        auto& segment = splitData2[index];
        shared_ptr<ByteArrayPackage> outPackage( nullptr );
        merger.tryMerge(segment, string("Group 2"), outPackage);
    }

    shared_ptr<ByteArrayPackage> result1( nullptr );
    shared_ptr<ByteArrayPackage> result2( nullptr );
    bool ret1 = merger.tryMerge( splitData1[ splitData1.size() - 1 ], string("Group 1"), result1 );
    bool ret2 = merger.tryMerge( splitData2[ splitData2.size() - 1 ], string("Group 2"), result2 );

    // Assert
    ASSERT_TRUE( ret1 );
    ASSERT_TRUE( ret2 );

    ASSERT_NE( result1.get(), nullptr );
    ASSERT_NE( result2.get(), nullptr );

    ASSERT_EQ( result1->value(), originalData1->value() );
    ASSERT_EQ( result2->value(), originalData2->value() );
}
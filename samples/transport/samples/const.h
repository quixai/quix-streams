#pragma once

#include <string>
#include "../utils/myThread.h"


class Const
{
    public:
        static const std::string PartitionedPackageTestTopic; // Note you need to create partitions yourself

        static const std::string PackageTopic;

        static const std::string MessagesTopic;

        static const std::string BrokerList;    
};
#pragma once

#include <string>
#include <memory>

#include "transport/codec/abstractCodec.h"
#include "transport/codec/codecId.h"
#include "transport/io/Package.h"
#include "transport/io/IModel.h"
#include "transport/fw/modelKey.h"
#include "transport/registry/codecRegistry.h"

class TestModelCodec: public Quix::Transport::AbstractCodec{
public:
    TestModelCodec();
    Quix::Transport::ByteArray serialize(const std::shared_ptr<Quix::Transport::IPackage> obj) const;
    const std::shared_ptr<Quix::Transport::IPackage> deserialize(const std::shared_ptr<Quix::Transport::RawBytePackage> rawPackage) const;
};

class TestModel : public Quix::Transport::IModel{
public:
    static TestModelCodec defaultCodec;
    const Quix::Transport::ModelKey modelKey() const
    {
        return Quix::Transport::ModelKey("TestModel");
    };
    bool operator==(const TestModel& other) const;
    static class _init
    {
        public:
        _init()
        {
            Quix::Transport::CodecRegistry::instance()->registerCodec(
                Quix::Transport::ModelKey("TestModel"), 
                &defaultCodec
            );
        }
    } _initializer;
};

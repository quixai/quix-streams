#pragma once

#include <string>
#include <memory>

#include "transport/codec/ICodec.h"
#include "transport/codec/codecId.h"
#include "transport/io/package.h"
#include "transport/io/IModel.h"
#include "transport/fw/modelKey.h"
#include "transport/registry/codecRegistry.h"

class TestModel : public Quix::Transport::IModel
{
    char datastr_[10];
    unsigned int number_;

    static Quix::Transport::BaseCodec* defaultCodec;
public:
    TestModel();

    const Quix::Transport::ModelKey modelKey() const
    {
        return Quix::Transport::ModelKey::forType<TestModel>();
    };

    bool operator==(const TestModel& other) const;

    static class _init
    {
    public:
        _init()
        {
            Quix::Transport::CodecRegistry::instance()->registerCodec(
                Quix::Transport::ModelKey::forType<TestModel>(), 
                defaultCodec
            );
        }
    } _initializer;

};

class TestModelCodec: public Quix::Transport::ICodec<TestModel>
{

public:
    /**
     * Serialize the object with the codec.
     */
    virtual Quix::Transport::ByteArray serialize(const TestModel& obj) const;
    /**
     * Deserialize the byte array with the codec.
     */
    virtual TestModel deserialize(const Quix::Transport::ByteArray& package) const;

};


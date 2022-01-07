#pragma once

#include <string>
#include <cstring>
#include <memory>
#include <functional>

#include "transport/codec/abstractCodec.h"
#include "transport/codec/codecId.h"
#include "transport/io/package.h"
#include "transport/io/byteArray.h"
#include "transport/io/IModel.h"
#include "transport/transportPublisher.h"
#include "transport/fw/modelKey.h"
#include "transport/registry/codecRegistry.h"

template<class T>
class PackageFactoryCodec : public Quix::Transport::AbstractCodec
{
    private:
    public:
        PackageFactoryCodec(): Quix::Transport::AbstractCodec( std::string( typeid(this).name() ) )
        {

        };

        Quix::Transport::ByteArray serialize(const std::shared_ptr< Quix::Transport::IPackage > obj) const
        {
            auto casted = std::dynamic_pointer_cast< Quix::Transport::Package<T> >(obj);

            //returns only the casted previous value since the obj variable should already be of byte array type
            Quix::Transport::ByteArray arr = Quix::Transport::ByteArray::initEmpty( sizeof(T) );
            memcpy(arr.data(), (void*)casted.get(), sizeof(T) );

            return arr;
        };

        const std::shared_ptr<Quix::Transport::IPackage> deserialize(const std::shared_ptr<Quix::Transport::ByteArrayPackage> package) const
        {
            //returns only the casted previous value since the package variable is being parent of IPackage

            T data;
            memcpy((void*)&data, (void*)package->value().data(), sizeof(T) );

            return std::shared_ptr<Quix::Transport::IPackage>( new Quix::Transport::Package<T>(package, data) );
        };

};



template<typename T1, typename std::enable_if<std::is_base_of<Quix::Transport::IModel, T1>::value>::type* = nullptr>
std::shared_ptr< Quix::Transport::IPackage > createPackage( const T1& value, std::shared_ptr< Quix::Transport::TransportContext > transportContext )
{
    //arrange
    static PackageFactoryCodec<T1> codec1;

    auto registry = Quix::Transport::CodecRegistry::instance();
    registry->registerCodec( Quix::Transport::ModelKey::forType<T1>() , &codec1 );

    std::shared_ptr< Quix::Transport::IPackage > result( nullptr );

    Passthrough input;
    input.onNewPackage += [&]( std::shared_ptr< Quix::Transport::IPackage > package ) {
        result = package;
    };

    Quix::Transport::TransportPublisher tInput(&input);

    auto pack = new Quix::Transport::Package<T1>(value, *transportContext );
    tInput.send( std::shared_ptr< Quix::Transport::IPackage >( pack ) );

    return result;
};


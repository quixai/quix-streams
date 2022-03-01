#pragma once

#include <string>
#include <cstring>
#include <memory>
#include <functional>

#include "transport/codec/binaryCodec.h"
#include "transport/codec/abstractCodec.h"
#include "transport/codec/codecId.h"
#include "transport/io/package.h"
#include "transport/io/byteArray.h"
#include "transport/io/IModel.h"
#include "transport/transportPublisher.h"
#include "transport/fw/modelKey.h"
#include "transport/registry/codecRegistry.h"

#include "./passthrough.h"

template<typename T1, typename std::enable_if<std::is_base_of<Quix::Transport::IModel, T1>::value>::type* = nullptr>
std::shared_ptr< Quix::Transport::IPackage > createPackage( const T1& value, std::shared_ptr< Quix::Transport::TransportContext > transportContext )
{
    //arrange
    static Quix::Transport::BinaryCodec<T1> codec1;

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


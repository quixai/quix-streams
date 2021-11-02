#pragma once


namespace Quix { namespace Transport {

class IInput{
};
class IByteSplitter{
};

class AbstractBasePackage{

};

class TransportWriter
{
  int gx;

public:
  TransportWriter(IInput* input, IByteSplitter* byteSplitter = nullptr);
  void Send(AbstractBasePackage* package);
};

} }

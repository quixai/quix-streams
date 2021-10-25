#pragma once

//! @namespace foo The Foo namespace
namespace transport {

class ObjWithPyCallback;

class Callback
{
   public:
      Callback(){}

      virtual ~Callback(){}
      virtual void call(ObjWithPyCallback& object){} 
};

}


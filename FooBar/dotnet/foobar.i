%module csFooBar

%include "std_string.i"
%include "std_map.i"
%include "base.i"

// Add necessary symbols to generated header
%{
#include <foobar/FooBar.hpp>
%}

%ignore "";
%define %unignore %rename("%s") %enddef

%unignore foobar;
namespace foobar {
%rename("FooBarHello") foobarHello(int);
%rename("FooBarHello") foobarHello(int64_t);

%unignore FooBar::FooBar();
%unignore FooBar::~FooBar();

%unignore FooBar;
%rename("Hello") FooBar::hello(int);
%rename("Hello") FooBar::hello(int64_t);

%rename("GetInt") FooBar::getInt() const;
%rename("SetBarInt") FooBar::setBarInt(int);
%rename("SetFooInt") FooBar::setFooInt(int);

%rename("GetInt64") FooBar::getInt64() const;
%rename("SetBarInt64") FooBar::setBarInt64(int64_t);
%rename("SetFooInt64") FooBar::setFooInt64(int64_t);

%rename ("ToString") FooBar::operator();
} // namespace foo

// Process symbols in header
%include "foobar/FooBar.hpp"

%unignore ""; // unignore all

%module pyFooBar

%include "stdint.i"
%include "std_string.i"
// Add necessary symbols to generated header
%{
#include <foobar/FooBar.hpp>
#include <transport/Transport.hpp>
%}

%ignore ""; // ignore all
%define %unignore %rename("%s") %enddef

%unignore foobar;
%unignore foobar::foobarHello(int);
%unignore foobar::foobarHello(int64_t);

%unignore foobar::FooBar;
%unignore foobar::FooBar::hello(int);
%unignore foobar::FooBar::hello(int64_t);

%unignore foobar::FooBar::getPDR() const;

%rename ("get_int") foobar::FooBar::getInt() const;
%rename ("set_int") foobar::FooBar::setBarInt(int);
%rename ("set_int") foobar::FooBar::setFooInt(int);

%rename ("get_int64") foobar::FooBar::getInt64() const;
%rename ("set_int64") foobar::FooBar::setBarInt64(int64_t);
%rename ("set_int64") foobar::FooBar::setFooInt64(int64_t);

%rename ("__str__") foobar::FooBar::operator();

// Process symbols in header
%include "foobar/FooBar.hpp"

%unignore ""; // unignore all

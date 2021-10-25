#include "transport/Transport.hpp"

#include <iostream>

namespace transport {

Transport::Transport() {}
Transport::~Transport() {}

void Transport::hello(int level) {
  std::cout << "[" << level << "] Enter Foo::hello(int)" << std::endl;
  std::cout << "[" << level << "] Exit Foo::hello(int)" << std::endl;
}

ParameterDataRaw Transport::getPDR() const{
  return ParameterDataRaw();
}

void Transport::hello(int64_t level) {
  std::cout << "[" << level << "] Enter Foo::hello(int64_t)" << std::endl;
  std::cout << "[" << level << "] Exit Foo::hello(int64_t)" << std::endl;
}

std::string Transport::operator()() const {
  return std::string{"\"Foo\":{\"int\":"} + std::to_string(_intValue) +
         ",\"int64\":" + std::to_string(_int64Value) + "}";
}

int Transport::getInt() const {
  return _intValue;
}

void Transport::setInt(int input) {
  _intValue = input;
}

int64_t Transport::getInt64() const {
  return _int64Value;
}

void Transport::setInt64(int64_t input) {
  _int64Value = input;
}

} // namespace foo

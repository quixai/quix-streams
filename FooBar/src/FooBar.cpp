#include "foobar/FooBar.hpp"

#include <iostream>

#include "transport/Transport.hpp"

namespace foobar {
void foobarHello(int level) {
  std::cout << "[" << level << "] Enter foobarHello(int)" << std::endl;
  std::cout << "[" << level << "] Exit foobarHello(int)" << std::endl;
}

void foobarHello(int64_t level) {
  std::cout << "[" << level << "] Enter foobarHello(int64_t)" << std::endl;
  std::cout << "[" << level << "] Exit foobarHello(int64_t)" << std::endl;
}

FooBar::FooBar(){}
FooBar::~FooBar(){}

void FooBar::hello(int level) {
  std::cout << "[" << level << "] Enter FooBar::hello(int)" << std::endl;
  transport::Transport::hello(level + 1);
  std::cout << "[" << level << "] Exit FooBar::hello(int)" << std::endl;
}

void FooBar::hello(int64_t level) {
  std::cout << "[" << level << "] Enter FooBar::hello(int64_t)" << std::endl;
  transport::Transport::hello(level + 1);
  std::cout << "[" << level << "] Exit FooBar::hello(int64_t)" << std::endl;
}

int FooBar::getInt() const {
  return _transport.getInt();
}

void FooBar::setFooInt(int input) {
  _transport.setInt(input);
}

void FooBar::setBarInt(int input) {
  _transport.setInt(input);
}

int64_t FooBar::getInt64() const {
  return _transport.getInt64();
}

transport::ParameterDataRaw FooBar::getPDR() const {
  return _transport.getPDR();
}


void FooBar::setBarInt64(int64_t input) {
  _transport.setInt64(input);
}

void FooBar::setFooInt64(int64_t input) {
  _transport.setInt64(input);
}

std::string FooBar::operator()() const {
  return std::string("\"FooBar\":{" + _transport() + "}");
}

} // namespace foobar

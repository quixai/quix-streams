#include <cstdint>
#include <foobar/FooBar.hpp>
#include <iostream>

int main(int /*argc*/, char** /*argv*/) {
  foobar::FooBar::hello(int{1});
  std::cout << std::endl;
  foobar::FooBar::hello(int64_t{1});
  std::cout << std::endl;

  foobar::FooBar f;
  f.setBarInt(13);
  f.setFooInt(17);

  f.setBarInt64(int64_t{31});
  f.setFooInt64(int64_t{42});

  std::cout << f() << std::endl;

  return 0;
}

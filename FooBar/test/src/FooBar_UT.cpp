#include <catch2/catch_all.hpp>
#include <foobar/FooBar.hpp>
#include <iostream>
#include <numeric>

namespace foobar {

TEST_CASE("FooBar static method", "[FooBar]") {
  SECTION("Int Method") { REQUIRE_NOTHROW(FooBar::hello(42)); }
  SECTION("Int64_t Method") { REQUIRE_NOTHROW(FooBar::hello(int64_t{42})); }
}

TEST_CASE("FooBar::Ctor", "[FooBar]") {
  SECTION("Default constructor") {
    FooBar* b = new FooBar();
    REQUIRE(b != nullptr);
  }
}

SCENARIO("FooBar Int", "[FooBar]") {
  GIVEN("A FooBar instance") {
    FooBar bar;
    WHEN("Setting a value") {
      REQUIRE_NOTHROW(bar.setBarInt(31));
      REQUIRE_NOTHROW(bar.setFooInt(42));
      THEN("The value is updated") { REQUIRE(bar.getInt() == 73); }
    }
  }
}

SCENARIO("FooBar Int64", "[FooBar]") {
  GIVEN("A FooBar instance") {
    FooBar bar;
    WHEN("Setting a value") {
      REQUIRE_NOTHROW(bar.setBarInt64(13));
      REQUIRE_NOTHROW(bar.setFooInt64(17));
      THEN("The value is updated") { REQUIRE(bar.getInt64() == 30); }
    }
  }
}

TEST_CASE("FooBar::operator()", "[FooBar]") {
  SECTION("Debug print") { INFO("FooBar: " << FooBar()()); }
}

} // namespace foobar

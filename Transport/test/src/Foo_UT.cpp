#include <catch2/catch_all.hpp>
#include <transport/Transport.hpp>
#include <iostream>
#include <numeric>

namespace transport {

TEST_CASE("Transport static method", "[Transport]") {
  SECTION("Int Method") { REQUIRE_NOTHROW(Transport::hello(42)); }
  SECTION("Int64_t Method") { REQUIRE_NOTHROW(Transport::hello(int64_t{42})); }
}

TEST_CASE("Transport::Ctor", "[Transport]") {
  SECTION("Default constructor") {
    Transport* b = new Transport();
    REQUIRE(b != nullptr);
  }
}

SCENARIO("Transport Int", "[Transport]") {
  GIVEN("A Foo instance") {
    Transport foo;
    WHEN("Setting a value") {
      REQUIRE_NOTHROW(foo.setInt(42));
      THEN("The value is updated") { REQUIRE(foo.getInt() == 42); }
    }
  }
}

SCENARIO("Transport Int64", "[Transport]") {
  GIVEN("A Transport instance") {
    Transport foo;
    WHEN("Setting a value") {
      REQUIRE_NOTHROW(foo.setInt64(31));
      THEN("The value is updated") { REQUIRE(foo.getInt64() == 31); }
    }
  }
}

TEST_CASE("Foo::operator()", "[Foo]") {
  SECTION("Debug print") { INFO("Foo: " << Transport()()); }
}

} // namespace foo

#include <doctest.hpp>
#include <small_vector/small_vector.h>
using doctest::test_suite;
using namespace sv;
#include <string>

TEST_CASE("Access elements in a small vector of ints" * test_suite("access")) {
  small_vector<int, 5> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // .operator[] access
  REQUIRE(a[0] == 1);
  REQUIRE(a[4] == 5);

  const auto a_7 = a[7];
  REQUIRE(a_7 == 8);

  auto& a_9 = a[9];
  REQUIRE(a_9 == 10);

  // .at() access
  REQUIRE(a.at(0) == 1);
  REQUIRE(a.at(9) == 10);
  REQUIRE_THROWS(a.at(14));

  // front() access
  REQUIRE(a.front() == 1);

  // back() access
  REQUIRE(a.back() == 10);
}
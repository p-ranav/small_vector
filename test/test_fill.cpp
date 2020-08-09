#include <doctest.hpp>
#include <small_vector/small_vector.h>
using doctest::test_suite;
using namespace sv;
#include <string>

TEST_CASE("Fill a small vector of ints with 0s" * test_suite("swap")) {
  small_vector<int, 5> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  a.fill(0);
  REQUIRE(a.size() == 10);
  std::size_t j = 0;
  for (int i = 0; i < 10; i++, j++) {
    REQUIRE(a[i] == 0);
  }
  REQUIRE(j == 10);
}
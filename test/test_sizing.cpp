#include <doctest.hpp>
#include <small_vector/small_vector.h>
using doctest::test_suite;
using namespace sv;
#include <string>

TEST_CASE("Sizing/Resizing in small vector of ints" * test_suite("sizing")) {
  small_vector<int, 5> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  REQUIRE_FALSE(a.empty());
  REQUIRE(a.size() == 10);

  a.resize(6);
  REQUIRE(a.size() == 6);

  a.resize(20);
  REQUIRE(a.size() == 20);
  std::size_t i = 0; 
  for (auto j = 6; j < a.size(); j++) {
    REQUIRE(a[j] == 0);
    i += 1;
  }
  REQUIRE(i == 14);
}
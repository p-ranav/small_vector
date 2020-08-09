#include <doctest.hpp>
#include <small_vector/small_vector.h>
using doctest::test_suite;
using namespace sv;
#include <string>

TEST_CASE("Pushing into a small vector" * test_suite("push_back")) {
  small_vector<int, 5> a;
  for (auto v: {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    a.push_back(v);
  }
  REQUIRE(a.size() == 9);
  std::size_t j = 0;
  for (int i = 0; i < a.size(); i++, j++) {
    REQUIRE(a[i] == i + 1);
  }
  REQUIRE(j == 9);
}

TEST_CASE("Popping out of a small vector" * test_suite("pop_back")) {
  small_vector<int, 5> a;
  for (auto v: {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
    a.push_back(v);
  }
  REQUIRE(a.size() == 9);

  // Remove 5 elements
  std::size_t i = 0;
  while(i < 5) {
    a.pop_back();
    i += 1;
  }
 
  // Now all data should be on the stack
  REQUIRE(a.size() == 4);
  REQUIRE(a[0] == 1);
  REQUIRE(a[1] == 2);
  REQUIRE(a[2] == 3);
  REQUIRE(a[3] == 4);
  
  // pop back more
  while(i < 10) {
    a.pop_back();
    i += 1;
  }

  REQUIRE(a.size() == 0);
  REQUIRE(a.empty());
}
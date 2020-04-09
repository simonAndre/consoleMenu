// tests-factorial.cpp
#include "../catch/catch.hpp"
#include "factorial.h"

TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    // REQUIRE(Factorial(4) == 556); //false
    REQUIRE(Factorial(10) == 3628800);
}
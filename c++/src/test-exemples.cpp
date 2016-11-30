#include <UnitTest++.h>

// Un test simple qui réussit
TEST(test1) {
  CHECK(2 == 1+1);
}

// Un test simple qui échoue
TEST(test2) {
  CHECK(3 == 1+2);
}

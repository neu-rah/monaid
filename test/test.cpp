#include <unity.h>

#include <lplus.h>

#include <vector>
#include <cstring>
#include <functional>
#include <type_traits>
#include <iostream>
using namespace std;

void test_concepts();
void test_monoid();
void test_monad();
void chk_either();

void setUp(void) {}
void tearDown(void) {}

int main() {
  UNITY_BEGIN();
  test_concepts();
  test_monoid();
  test_monad();
  RUN_TEST(chk_either);
  UNITY_END();
}

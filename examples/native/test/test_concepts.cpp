#include <unity.h>

#include <monaid.h>

#include <vector>
#include <cstring>
using namespace std;

void chk_aString() {
  TEST_ASSERT_TRUE(aString<decltype(string(""))>);
  TEST_ASSERT_FALSE(aString<Nil>);
  TEST_ASSERT_FALSE(aString<const char*>);
}

void chk_aMonoid() {
  TEST_ASSERT_TRUE(aMonoid<Container<vector<int>>>);
  TEST_ASSERT_FALSE(aMonoid<string>);
  TEST_ASSERT_FALSE(aMonoid<int>);
}

void chk_aFunctor() {
  TEST_ASSERT_TRUE(aFunctor<Maybe<Nil>>);
  TEST_ASSERT_TRUE(aFunctor<Container<vector<Nil>>>);
  TEST_ASSERT_FALSE(aFunctor<string>);
  TEST_ASSERT_FALSE(aFunctor<int>);
}

void chk_aFoldable() {
  TEST_ASSERT_TRUE(aFoldable<Container<vector<int>>>);
  //change this to be TRUE, lets fold naked containers and monadic functors
  TEST_ASSERT_FALSE(aFoldable<Maybe<Nil>>);
  TEST_ASSERT_FALSE(aFoldable<string>);

  TEST_ASSERT_FALSE(aFoldable<int>);
}

void chk_aContainer() {
  TEST_ASSERT_TRUE(aContainer<string>);
  TEST_ASSERT_TRUE(aContainer<Container<string>>);
  TEST_ASSERT_TRUE(aContainer<vector<float>>);
  TEST_ASSERT_FALSE(aContainer<Maybe<Nil>>);
}

void chk_aNakedContainer() {
  TEST_ASSERT_TRUE (aNakedContainer<string>);
  TEST_ASSERT_TRUE (aNakedContainer<vector<Nil>>);
  TEST_ASSERT_FALSE(aNakedContainer<Container<string>>);
  TEST_ASSERT_FALSE(aNakedContainer<Container<vector<bool>>>);
  TEST_ASSERT_FALSE(aNakedContainer<Nil>);
  TEST_ASSERT_FALSE(aNakedContainer<int>);
}

void test_concepts() {
  RUN_TEST(chk_aString);
  RUN_TEST(chk_aMonoid);
  RUN_TEST(chk_aFunctor);
  RUN_TEST(chk_aFoldable);
  RUN_TEST(chk_aContainer);
  RUN_TEST(chk_aNakedContainer);
}

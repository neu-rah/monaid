#include <unity.h>

#include <lplus.h>

#include <vector>
#include <cstring>
#include <functional>
#include <type_traits>
#include <iostream>
using namespace std;

void chk_pure(){
  auto x=pure(11);
  TEST_ASSERT_TRUE(pure(true));
  TEST_ASSERT_TRUE((int)pure(1967)==1967);
  TEST_ASSERT_TRUE(isJust((Maybe<int>)pure(1967)));
  TEST_ASSERT_TRUE((fromJust((Maybe<int>)x)==11));
}

auto aa(auto o){return o*0.75;}
auto am(auto o){return Maybe(aa(o));}

void chk_fmap() {
  TEST_ASSERT_TRUE_MESSAGE(
    (is_same_v<Maybe<>,nrt$((Maybe<>)(mempty.fmap(aa<int>)))>),
    "accumulate fmap operation on mempty");
  TEST_ASSERT_TRUE_MESSAGE(
    (is_same_v<Maybe<float>,nrt$((Maybe<float>)(mempty.fmap(aa<int>)))>),
    "accumulate fmap operation over mempty with type transition");
  TEST_ASSERT_TRUE_MESSAGE(
    (fromJust((Maybe<float>)(pure(2).fmap(aa<int>)))==1.5),
    "int->float over pure");
}

void chk_bind() {
  TEST_ASSERT_TRUE_MESSAGE(
    isNothing((Maybe<>)(mempty.bind(am<int>))),
    "bind over mempty");
  TEST_ASSERT_TRUE_MESSAGE(
    fromJust( (Maybe<double>) (pure(2).bind(am<int>)) )==1.5,
    "bind over pure");
  TEST_ASSERT_TRUE_MESSAGE(
    fromJust( Just(2).bind(am<int>) )==1.5,
    "bind over Maybe");
}

const auto add=$([](auto a,auto b){return a+b;});

void chk_liftM2() {
  TEST_ASSERT_TRUE_MESSAGE(
    (fromJust(liftM2(add,Just(5),Just(8)))==13),
    "liftM2 to Maybe");
  TEST_ASSERT_TRUE_MESSAGE(
    (fromJust(liftM2(add,Just(5),Just(4.2)))==9.2),
    "liftM2 type transformer");
  TEST_ASSERT_TRUE_MESSAGE(
    (isNothing(liftM2(add,Maybe<int>(),Maybe(8)))),
    "first absorvent");
  TEST_ASSERT_TRUE_MESSAGE(
    (isNothing(liftM2(add,Maybe(5),Maybe<int>()))),
    "second absorvent");
  TEST_ASSERT_TRUE_MESSAGE(
    (isNothing(liftM2(add,Maybe<int>(),Maybe<int>()))),
    "all absorvents");
  
  const auto x3=[](auto o){return pure(o*3);};
  const auto willFail=[](auto){return mempty;};
  auto comp1=Just(2).bind(x3).bind(willFail).bind(am<int>);
  TEST_ASSERT_TRUE_MESSAGE(
    (isNothing(comp1)),
    "monad with bind operator, fail test");
  auto comp2=Just(2).bind(x3).bind(am<int>);
  TEST_ASSERT_TRUE_MESSAGE(
    (isJust(comp2)),
    "monad with bind operator, success test");
}

void test_monad() {
  RUN_TEST(chk_pure);
  RUN_TEST(chk_fmap);
  RUN_TEST(chk_bind);
  RUN_TEST(chk_liftM2);
}

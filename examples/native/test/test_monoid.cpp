#include <unity.h>

#include <monaid.h>

#include <vector>
#include <cstring>
#include <functional>
#include <type_traits>
#include <iostream>
using namespace std;

void chk_mempty(){
  Maybe<int> a=mempty;
  TEST_ASSERT_TRUE((is_same_v< decltype((Maybe<>)mempty),Maybe<> >));
  TEST_ASSERT_TRUE((is_same_v< decltype((Container<vector<int>>)mempty),Container<vector<int>> >));
  TEST_ASSERT_TRUE((is_same_v< nrt$(a),Maybe<int> >));
  TEST_ASSERT_TRUE((is_same_v<decltype((int)mempty),int >));
}

void chk_mappend() {
  using Str=Container<string>;
  TEST_ASSERT_TRUE_MESSAGE(
    (mappend(Str("ok"))(Str("fail"))==string("okfail")),
    "mappend string containers"
  );
  auto a=Just(Container<vector<int>>{1,2});
  auto b=Just(Container<vector<int>>{3,4,5});
  TEST_ASSERT_TRUE_MESSAGE(
    (mappend(a,b).size()==fromJust(a).size()+fromJust(b).size()),
    "mappend over Maybe<vector containers>\nMonoids should mappend as owned values can."
  );
  TEST_ASSERT_TRUE_MESSAGE(
    (mappend(string("yes|"),string("|no")).length()==7),
    "mappend over strings..."
  );
}

void test_monoid() {
  RUN_TEST(chk_mempty);
  RUN_TEST(chk_mappend);
}

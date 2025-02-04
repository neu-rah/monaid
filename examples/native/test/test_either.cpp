#include <unity.h>

#include <monaid.h>
#include <cstring>
using namespace std;

void chk_either(){
  Either<int,const char*> ok(1967);
  Either<int,const char*> fail("fail");
  TEST_ASSERT_TRUE_MESSAGE((fromRight(ok.fmap([](auto o){return 2025-o;}))==58),"Right fmap");
  TEST_ASSERT_TRUE_MESSAGE((string("fail")==fromLeft(fail.fmap([](auto o){return 2025-o;}))),"Left fmap");

  Right<string,string> a(string("ok"));
  Right<string,string> b(", let's go...");
  Left<string,string> c("fail");
  TEST_ASSERT_TRUE_MESSAGE((string("ok, let's go...")==fromRight(a.mappend(b))),"mappend Right Right");
  TEST_ASSERT_TRUE_MESSAGE((string("ok")==fromRight(a.mappend(c))),"mappend Right Left");
  TEST_ASSERT_TRUE_MESSAGE((string("ok")==fromRight(c.mappend(a))),"mappend Left Right");
}


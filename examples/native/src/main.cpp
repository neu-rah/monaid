#include <monaid.h>
#include <iostream>
using namespace std;

auto _const{$([](auto o,auto) {return o;})};

int main() {
  cout<<(
    Just(2)
      >>( [](auto o) {return pure(2*o);})
      >>(_const(mempty))//computation that will fail!
      >>( [](auto o) {return pure((double)3.1*o);} )//changing type int->double
  )<<endl;
  return 0;
}
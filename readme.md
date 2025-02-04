# Monoids and monads under C++23

Implemented this stuff while exploring C++23, just for fun


## examples

**successful computation**
```c++
#include <monaid.h>
#include <iostream>
using namespace std;

int main() {
  cout<<(
    Just(2)
      >>( [](auto o) {return pure(2*o);})
      >>( [](auto o) {return pure((double)3.1*o);} )//changing type int->double
  )<<endl;
  return 0;
}
```
output:
```
Just 12.4
```

**failing computation**
```c++
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
```
output:
```
Nothing
```

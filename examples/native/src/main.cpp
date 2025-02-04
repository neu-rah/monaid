#include <monaid.h>
#include <iostream>
using namespace std;

int main() {
  cout<<(
    Just(2)
      >>( [](auto o) {return 2*o;})
      >>( [](auto o) {return 3.1*o;})
  )<<endl;
  return 0;
}
#include <iostream>
// #include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;

#include <monaid.h>

cex const auto e0{id("ok")};
cex const auto e1{_true("ok")("fail")};
cex const auto e2{_true(id)("fail")("ok")};
cex const auto e3{n2(cons("."))(cons("<-")(cons("last")("!")))(_false)(_false)(_false)(_true)};
cex const auto r3{beta(e3)};

auto bench(const auto& f) {
  auto start1 = high_resolution_clock::now();
  const long int cnt=10000;
  for(long int n=0;n<cnt;n++) beta(f);
  auto stop1 = high_resolution_clock::now();
  auto duration=duration_cast<microseconds>(stop1-start1);
  cout<<((float)duration.count())/cnt<<"μs"<<endl;
}

int main() {
  cout<<"start ------------------"<<endl;
  cout<<e0<<"=> "<<beta(e0)<<endl;
  cout<<e1<<"=> "<<beta(e1)<<endl;
  cout<<e2<<"=> "<<beta(e2)<<endl;
  cout<<beta(_false("fail")("ok"))<<endl;
  cout<<beta(cons(1967,11)(_false))<<endl;
  cout<<beta(cons(".")(cons(1967)(11))(_false)(_false))<<endl;
  cout<<beta(e3)<<endl;
  bench(r3);
  bench(e3);
  bench(n2(cons("."))(cons("<-")(cons("last")("!")))(_false)(_false)(_false)(_true));
  return 0;
}  
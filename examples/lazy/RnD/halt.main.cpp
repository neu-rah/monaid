#include <iostream>
using namespace std;
#include <monaid/base.h>

#define cex constexpr

template<typename C> struct Combinator {};

template<typename F>
struct App {
  const F beta;
  // cex const auto operator()(const auto&... oo) const {return operator()(move(oo)...);}
  cex const auto operator()(const auto... oo) const {
    // (cout<<...<<oo)<<"+";
    return ::App{[this,oo...](const auto... pp){return beta(oo...,pp...);}};
  }
};

struct I {
  static cex const auto& beta(const auto& o){return o;}
  static cex const auto operator()(const auto... oo)
    {return App{[oo...](const auto... pp){return beta(oo...,pp...);}};}
};cex const I id;

struct K {
  static cex const auto& beta(const auto& o,const auto&){return o;}
  static cex const auto operator()(const auto... oo) {
    // (cout<<"K+"<<...<<oo);
    return App{[oo...](const auto... pp){return beta(oo...,pp...);}};
  }
};cex const K _true;

// cex const auto e0{id(1967)};
constexpr const int day=11;
constexpr const char* name="Rui";
const auto e1{_true(day)(name)};

int main() {
  // cout<<e0.beta()<<endl;
  cout<<"=>"<<e1.beta()<<endl;
  cout<<"=>"<<_true(11)("Rui").beta()<<endl;
  cout<<"end."<<endl;
  return 0;
}
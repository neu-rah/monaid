#include <iostream>
using namespace std;

#include <monaid/curry.h>

template<typename O,typename... OO> struct Prog;

template<typename O> struct Prog<O> {
  using Op=O;
  const Op op;
};

template<typename O,typename... OO> 
struct Prog:Prog<O> {
  using Prog<O>::op;
  using Tail=Prog<OO...>;
  const Tail tail;
  constexpr Prog(const O o,const OO... oo):Prog<O>{o},tail{oo...}{}
  constexpr Prog(const O o,const Tail oo):Prog<O>{o},tail{oo}{}
};

template<typename C> struct Combinator {};

struct I:Combinator<I> {
  static constexpr const auto beta(const auto o) {return o;}
};constexpr const I id;

Prog<int,bool,const char*> x{1,true,"ok"};

template<typename Out> Out& operator<<(Out& out,const I) {return out<<"id";}

int main() {
  cout<<id<<endl;
 return 0;
}
#include <iostream>
using namespace std;

#include <monaid/curry.h>
#include "monaid/maybe.h"

template<typename O,typename... OO> struct Prog;

template<typename O> struct Prog<O> {
  using Op=O;
  const Op op;
  template<typename F, typename... OO>
  const auto beta(const F f,const OO...oo) const {
    if constexpr (std::is_invocable_v<
        decltype(F::template beta<OO...,Op>),
        const decltype(oo)...,
        const decltype(op)
    >) return Just(f.beta(oo...,op));
    else return mempty;
  }
};

template<typename O,typename... OO> 
struct Prog:Prog<O> {
  using Prog<O>::op;
  using Tail=Prog<OO...>;
  const Tail tail;
  constexpr Prog(const O o,const OO... oo):Prog<O>{o},tail{oo...}{}
  constexpr Prog(const O o,const Tail oo):Prog<O>{o},tail{oo}{}
  const auto beta() const {return tail.beta(op);}
  const auto beta(const auto... oo) const {return tail.beta(oo...,op);}
};

template<typename C> struct Combinator {
  static constexpr const auto beta(const auto...) {return mempty;}
};

struct I:Combinator<I> {
  // using Combinator<I>::beta;
  static constexpr const auto beta(const auto o) {return o;}
};constexpr const I id;

struct K:Combinator<K> {
  // using Combinator<K>::beta;
  static constexpr const auto beta(const auto o,const auto) {return o;}
};constexpr const K _true;

template<typename Out,typename O>
Out& operator<<(Out& out,const Prog<O> o) {return out<<o.op;}
template<typename Out,typename O,typename... OO>
Out& operator<<(Out& out,const Prog<O,OO...> o) {return out<<o.op<<" "<<o.tail;}
template<typename Out> Out& operator<<(Out& out,const I) {return out<<"id";}
template<typename Out> Out& operator<<(Out& out,const K) {return out<<"true";}

namespace test {
  constexpr const Prog id(::id,"ok");
  constexpr const Prog _true(::_true,"ok","false");
  constexpr const Prog fail(::_true,"ok");
};

int main() {
  cout<<test::id<<" => ";
  cout<<test::id.beta()<<endl;
  cout<<test::_true<<" => "<<test::_true.beta()<<endl;
  // cout<<test::fail<<" => "<<test::fail.beta()<<endl;
  return 0;
}
#include <iostream>
#include <concepts>
using namespace std;

#define cex constexpr

//this works but there is a LOT of copy back and forth

#include <monaid/curry.h>
#include "monaid/maybe.h"

// template<typename O,typename... OO> struct Prog;

template<typename O,typename... OO>
struct Prog {
  const O head;
  using Tail=Prog<OO...>;
  const Tail tail;
  cex Prog(const O o, const OO... oo):head{o},tail{oo...}{}
  cex Prog(const O o, const Prog<OO...> oo):head{o},tail{oo}{}
  // cex Prog(const O o, const Prog<OO...>& oo):head{o},tail{oo}{}
  static cex int len() {return Tail::len()+1;}
  auto beta() const {return tail.template run<O::cnt-1>(head);}
  template<int n> requires(n==0)
  auto run(const auto f,const auto... oo) const {
    return Prog(f.beta(oo...,head),tail);
  }    
  template<int n> requires(n!=0)
  auto run(const auto f,const auto... oo) const {
    return tail.template run<n-1>(f,oo...,head);
  }
};

template<typename O>
struct Prog<O> {
  const O head;
  cex Prog(const O o):head{o}{}
  static cex int len() {return 1;}
  template<int n> requires(n==0)
  auto run(const auto f,const auto... oo) const 
    {return f.beta(oo...,head);}
};

//------------------------------------------------------------------------
namespace Combinators {
  template<typename C,int n> struct Combinator {
    static cex const int cnt=n;
    static cex const Empty beta(const auto... oo)
      {(cout<<...<<oo);return mempty;}
  };
  
  struct I:Combinator<I,1> {
    using Combinator<I,1>::beta;
    static cex const auto beta(const auto o) {return o;}
  };cex const I id;
  
  struct K:Combinator<K,2> {
    using Combinator<K,2>::beta;
    static cex const auto beta(const auto o,const auto) {return o;}
  };cex const K _true;
  
  template<typename Out,typename O>
  Out& operator<<(Out& out,const Prog<O> o)
    {return out<<o.head;}
  template<typename Out,typename O,typename... OO>
  Out& operator<<(Out& out,const Prog<O,OO...> o)
    {return out<<o.head<<" ("<<(void*)&o.tail<<")"<<o.tail;}
  template<typename Out> Out& operator<<(Out& out,const I) {return out<<"id";}
  template<typename Out> Out& operator<<(Out& out,const K) {return out<<"true";}
}; using namespace Combinators;
  
namespace t {
  cex const Prog id{::id,"ok","zZz"};
  // cex const Prog _true{::_true,"ok","fail","zZz","..."};
};

int main() {
  cout<<t::id<<" => "<<t::id.beta()<<endl;
  // cout<<t::_true<<" => ";
  // cout<<t::_true.beta()<<endl;
  // return 0;
}
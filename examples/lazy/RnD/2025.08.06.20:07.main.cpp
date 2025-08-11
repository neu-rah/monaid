#include <iostream>
#include <type_traits>
using namespace std;

#define cex constexpr

struct LambdaExpr{};
template<typename E> cex const bool isExpr{is_convertible_v<E,LambdaExpr>};

template<typename O,typename... OO>
struct Expr:LambdaExpr {
  using Head=O;
  using Tail=Expr<OO...>;
  const Head head;
  const Tail tail;
  cex Expr(const Head o,const OO... oo):head{o},tail{oo...}{}
  cex const auto operator()(const auto o) const 
    {return ::Expr{head,tail(o)};}
  cex const auto beta() const {return head.run(tail);}
  cex const auto run(const auto f,const auto... pp) const {return tail.run(f,pp...,head);}
};

template<typename O,typename P>
struct Expr<O,P> {
  using Head=O;
  const Head head;
  using Tail=P;
  const Tail tail;
  cex const auto operator()(const auto o) const
    {return ::Expr{head,tail,o};}
  cex const auto beta() const {return head.run(tail);}
  // cex const auto run(const auto f,const auto... pp) const {return f.run(pp...,head,tail);}
};

struct Lambda {};
template<typename C> cex const bool isLambda{is_convertible_v<C,Lambda>};
template<typename C> struct Combinator:Lambda {
  static cex const C op{};
  cex const auto operator()(const auto o) const {return Expr{op,o};}
  template<typename O,typename... OO>
  static cex const auto run(const Expr<O,OO...>& o,const auto... pp) {
    if cex(is_invocable_v<decltype(C::template beta<decltype(pp)...>),decltype(pp)...>)
      return o.beta(pp...);
    else return run(o.tail,pp...,o.head);
  }
};

struct I:Combinator<I> {
  static cex const auto& beta(const auto& o){return o;}
};cex const auto& id{Combinator<I>::op};

struct K:Combinator<K> {
  static cex const auto& beta(const auto& o,const auto&){return o;}
};cex const auto& _true{Combinator<K>::op};

namespace print {
  #ifdef DEBUG
    template<typename Out> Out& operator<<(Out& out,const I& o){return out<<"("<<&o<<":id)";}
    template<typename Out> Out& operator<<(Out& out,const K& o){return out<<"("<<&o<<":true)";}
    template<typename Out,typename O,typename... OO>
    Out& operator<<(Out& out,const Expr<O,OO...>& o) 
      {return out<<"{"<<&o.head<<"#"<<o.head<<" "<<o.tail<<"}";}
    template<typename Out,typename O,typename P>
    Out& operator<<(Out& out,const Expr<O,P>& o) requires(!isExpr<P>)
      {return out<<"{"<<&o.head<<"#"<<o.head<<" "<<&o.tail<<"#"<<o.tail<<"}";}
  #else
    template<typename Out> Out& operator<<(Out& out,const I& o){return out<<"id";}
    template<typename Out> Out& operator<<(Out& out,const K& o){return out<<"true";}
    template<typename Out,typename O,typename... OO>
    Out& operator<<(Out& out,const Expr<O,OO...>& o) 
      {return out<<o.head<<" "<<o.tail;}
  #endif
}; using namespace print;

cex const auto e0{id("ok")("zZz")};
cex const auto e1{_true("ok")("fail")};

int main() {
  cout<<id<<_true<<endl;
  cout<<e0<<" => "<<e0.beta()<<endl;
  cout<<e1<<" => "<<e1.beta()<<endl;
  return 0;
}
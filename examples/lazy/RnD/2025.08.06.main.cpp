#include <iostream>
#include <type_traits>
using namespace std;

#define cex constexpr

struct Lambda{}; template<typename C> cex const bool isLambda{is_convertible_v<C,Lambda>};

struct Expr{}; template<typename E> cex const bool isExpr{is_convertible_v<E,Expr>};

template<typename O,typename... OO>
struct Prog:Expr {
  using Head=O;
  using Tail=Prog<OO...>;
  const Head head;
  const Tail tail;
  cex Prog(const O o,const OO... oo):head{o},tail{oo...}{}
  cex Prog(const O o,const Prog<OO...> oo):head{o},tail{oo}{cout<<"{OO...}"<<endl;}
  cex const auto operator()(const auto o) const 
    {cout<<*this<<"("<<o<<")"<<endl;auto r=Prog<Head,const decltype(tail(o))>{head,tail(o)};cout<<"->"<<r<<endl;return r;;}
};

template<typename O>
struct Prog<O>:Expr {
  using Head=O;
  const Head head;
  cex Prog(const O o):head{o}{}
  cex const auto operator()(const auto o) const {
    if cex (isLambda<Head>) {cout<<*this<<"("<<o<<")!"<<endl;auto r=head(o);cout<<"->"<<r<<endl;return r;}
    else {cout<<*this<<"("<<o<<")$"<<endl;auto r=Prog<Head,const decltype(o)>{head,o};cout<<"->"<<r<<endl;return r;}
  }
};

template<typename Out,typename... OO>
Out& operator<<(Out& out,const Prog<OO...>& o) {return out<<"{"<<o.head<<" "<<o.tail<<"}";}

template<typename Out,typename O>
Out& operator<<(Out& out,const Prog<O>& o) {return out<<"{"<<o.head<<"}";}

cex Prog rui{1967,"Rui"};

template<typename C>
struct Combinator:Lambda {
  static cex const C op{};
  static cex const auto operator()(const auto o){cout<<op<<"("<<o<<")"<<endl;return Prog{op,o};}
};

struct I:Combinator<I> {
  static cex const auto beta(const auto& o){return o;}
}; cex const I id;
template<typename Out> Out& operator<<(Out& out,const I&){return out<<"id";}

struct K:Combinator<K> {
  static cex const auto beta(const auto& o){return o;}
}; cex const K _true;
template<typename Out> Out& operator<<(Out& out,const K&){return out<<"true";}


int main() {
  const auto e0=_true("ok");
  const auto e1=e0("fail");
  cout<<endl;
  cout<<e1<<endl;
  // cout<<rui<<endl;  
  // cout<<id("ok")<<endl;
  // cout<<_true("ok")("fail")<<endl;
  cout<<"end."<<endl;
  return 0;
}
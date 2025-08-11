#include <iostream>
#include <type_traits>
using namespace std;

#define cex constexpr

struct Lambda{};
template<typename O> cex const bool isLambda{is_convertible_v<O,Lambda>};

template<int n,typename C>
struct Combinator:Lambda {
  static cex const int cnt{n};
  static cex const auto operator()(const auto&... oo) requires(n==1);
  static cex const auto operator()(const auto&... oo) requires(n!=1);
};

template<typename O,typename... OO> struct Expr {
  const O o;
  const Expr<OO...> oo;
};

template<int n,typename E>
struct App {//}:Combinator<App<E>> {
  const E expr;
  cex const auto operator()(const auto&... oo) const  requires(n!=1) {
    // (cout<<...<<oo)<<"-"<<n<<"^";
    const auto f=[this,oo...](const auto... pp){return expr(oo...,pp...);};
    return ::App<n-sizeof...(oo),decltype(f)>{f};
  }
  cex const auto operator()(const auto&... oo) const requires(n==1) {
    // (cout<<...<<oo)<<n<<"¹";
    if cex(isLambda<decltype(expr(oo...))>) {
      const auto f=[this,oo...](const auto... pp){return expr(oo...)(pp...);};
      return ::App<n-sizeof...(oo),decltype(f)>{f};
    } else {
      const auto f=[this,oo...](){return expr(oo...);};
      return ::App<0,decltype(f)>{f};
    }
  }
  cex const auto beta(const auto... oo) const {return expr(oo...);}
};

template<int n,typename C>
cex const auto Combinator<n,C>::operator()(const auto&... oo) requires(n!=1){
  // (cout<<...<<oo)<<"-"<<n<<"⁰";
  const auto f=[oo...](const auto... pp){return C::beta(oo...,pp...);};
  return ::App<0,decltype(f)>{f};
}

template<int n,typename C>
cex const auto Combinator<n,C>::operator()(const auto&... oo) requires(n==1) {
  if cex(isLambda<decltype(C::beta(oo...))>) {
    const auto f=[oo...](const auto... pp){return C::beta(oo...)(pp...);};
    return ::App<decltype(C::beta(oo...))::cnt,decltype(f)>{f};
  } else {
    const auto f=[oo...](){return C::beta(oo...);};
    return ::App<0,decltype(f)>{f};
  }
}

struct I:Combinator<1,I> {static cex const auto beta(const auto o) {return o;}};cex const I id;
struct K:Combinator<2,K> {static cex const auto beta(const auto o,const auto) {return o;}};cex const K _true;

template<typename Out> Out& operator<<(Out& out,const I&) {return out<<"id";}
template<typename Out> Out& operator<<(Out& out,const K&) {return out<<"true";}
template<typename Out,int n,typename E> Out& operator<<(Out& out,const App<n,E>& e) {return out<<"λ";}

const auto e0=id("ok");
const auto e1=_true("ok")("fail");
const auto e2=_true(id)("fail")("ok");

int main() {
  // cout<<e0<<" => "<<e0.beta()<<endl;
  cout<<e1<<" => ";
  cout<<e1.beta()<<endl;
  // cout<<e2<<" => "<<e2.beta().beta()<<endl;
  cout<<"end."<<endl;
  return 0;
}
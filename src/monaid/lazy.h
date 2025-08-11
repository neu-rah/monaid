#pragma once

#include <type_traits>
using std::is_invocable_v;
using std::is_convertible_v;

struct IsExpr {};
template<typename O> constexpr const bool isExpr{is_convertible_v<O,IsExpr>};

template<int n,typename B>
struct Expr:IsExpr {
  static constexpr const int cnt{n};
  using Beta=B;
  const Beta beta;
  constexpr Expr(const Beta b):beta{b}{}
  constexpr const auto operator()(const auto&... oo) const {return app(beta,oo...);}
private:
  static constexpr const auto app(const auto b,const auto... oo) requires(n==0) {
    //lazy combinator is complete, add a local beta when trying to add more parameters, and then add a continuation beta
    const auto f{[b,oo...](const auto&... pp){return b()(oo...,pp...).beta();}};
    return ::Expr<decltype(b())::cnt-(int)sizeof...(oo),decltype(f)>{f};
  }
  static constexpr const auto app(const auto b,const auto o,const auto... oo) requires(n!=0) {
    //need more parameters...
    if constexpr(sizeof...(oo)+1>n) {//tomm much? add one at a time then
      const auto f{[b,o,oo...](const auto&... pp){return app(b,o)(oo...,pp...);}};
      return ::Expr<n-sizeof...(oo)-1,decltype(f)>{f};
    } else {//just add them
      const auto f{[b,o,oo...](const auto&... pp){return b(o,oo...,pp...);}};
      return ::Expr<n-sizeof...(oo)-1,decltype(f)>{f};
    }
  }
};

template<typename Out,int n,typename B> 
Out& operator<<(Out& out,const Expr<n,B>& o) {return out<<"β";}

template<int n,auto b> constexpr const auto expr(){return Expr<n,decltype(b)>{b};}

constexpr const auto& beta(const auto& o) {return o;}
constexpr const auto beta(const auto& o) requires(isExpr<decltype(o)>) {return beta(o.beta());}

using I=decltype(expr<1,[](const auto& o){return o;}>());

template<auto f> constexpr const auto expr(const auto&... oo) requires(is_invocable_v<decltype(f),I>)         {return Expr<1,decltype(f),decltype(oo)...>{f,oo...};}
template<auto f> constexpr const auto expr(const auto&... oo) requires(is_invocable_v<decltype(f),I,I>)       {return Expr<2,decltype(f),decltype(oo)...>{f,oo...};}
template<auto f> constexpr const auto expr(const auto&... oo) requires(is_invocable_v<decltype(f),I,I,I>)     {return Expr<3,decltype(f),decltype(oo)...>{f,oo...};}
template<auto f> constexpr const auto expr(const auto&... oo) requires(is_invocable_v<decltype(f),I,I,I,I>)   {return Expr<4,decltype(f),decltype(oo)...>{f,oo...};}
template<auto f> constexpr const auto expr(const auto&... oo) requires(is_invocable_v<decltype(f),I,I,I,I,I>) {return Expr<5,decltype(f),decltype(oo)...>{f,oo...};}

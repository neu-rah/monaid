#pragma once

#define cex constexpr

// template<typename O,typename... OO> struct Prog;
// template<typename O,typename N> struct Expr;

namespace Lazy {
  struct LambdaExpr {};
  template<typename O>
  constexpr const bool isLambdaExpr=is_base_of_v<LambdaExpr,O>;

  //runtime interpreted version ------------
  template<typename O,typename N>
  struct Expr:LambdaExpr {
    using Op=O;
    const Op op;
    using Next=N;
    const Next next;
    Expr(const O o, const N n):op{o},next{n}{}
    constexpr size_t sz() const {return 1;}
    constexpr const auto operator()(const auto o) const
      {return Expr<Op,Expr<Next,decltype(o)>>{op,{next,o}};}
    template<int n> requires(n>0)
    const auto run(const auto& f,const auto&... pp) const
      {return f.beta(f,pp...,op,next);}
    template<int n> requires(n==0)
    const auto run(const auto& f,const auto&... pp) const;
  };

  template<typename O, typename N>
  requires(isLambdaExpr<N>)//finally, Expr<O,Expr<*,N>> just didnt work
  struct Expr<O,N>:LambdaExpr {
    using Op=O;
    const Op op;
    using Next=N;
    const Next next;
    Expr(const O o, const N n):op{o},next{n}{}
    constexpr size_t sz() const{return 1+next.sz();}
    constexpr const auto operator()(const auto o) const 
      {return Expr<Op,decltype(next(o))>{op,next(o)};}
    template<int n> requires(n>0)
    const auto run(const auto& f,const auto&... pp) const
      {return next.template run<n-1>(f,pp...,op);}
    template<int n> requires(n==0)
    const auto& run(const auto f,const auto... pp) const
      {return f.beta(pp...,op);}
  };

};

namespace Combinators {
  struct Lambda{};
  template<typename O>
  constexpr const bool isLambda=is_convertible_v<O,Lambda>;

  template<typename F>
  constexpr bool isCombinator=is_convertible_v<F,Lambda>;

  using namespace Lazy;
  template<typename C,int n>
  struct Combinator:Lambda {
    static cex const int cnt=n;
    static constexpr const auto operator()(const auto o) {return Expr<C,decltype(o)>{C{},o};}
  };
 
};

namespace Lazy {
  using namespace Combinators;
  //compiled code -------------
  template<typename O,typename... OO>
  struct Prog {
    using Head=O;
    using Tail=Prog<OO...>;
    const Head head;
    const Tail tail;
    cex Prog(const O o,const OO... oo):tail{oo...},head{o}{}
    template<int n> requires(n>0)
    const auto run(const auto& f,const auto&... pp) const
      {return tail.template run<n-1>(f,pp...,head);}
    template<int n> requires(n==0)
    const auto run(const auto& f,const auto&... pp) const
      {return beta(f.beta(pp...,head),tail);}
    // constexpr const auto operator()(const auto o) const {return Prog<Head,decltype(tail(o))>{head,tail(o)};}
  };

  template<typename O>
  struct Prog<O> {
    using Head=O;
    const Head head;
    cex Prog(const O o):head{o}{}
    template<int n> requires(n==0)
    const auto& run(const auto f,const auto... pp) const
      {return f.beta(pp...,head);}
    // constexpr const auto operator()(const auto o) const {return Prog<Head,decltype(o)>{head,o};}
  };

  //beta -------------
  template<typename O,typename... OO>
  const auto beta(const Prog<O,OO...>& p) requires(O::cnt<=sizeof...(OO))
    {return p.tail.template run<O::cnt-1>(p.head);}

  template<typename F,typename O,typename... OO>
  requires(isCombinator<F>&&F::cnt<=sizeof...(OO)+1)
  const auto beta(const F f,const Prog<O,OO...>& p) 
    {return p.template run<F::cnt-1>(f);}
    
  template<typename O,typename N>
  const auto beta(const Expr<O,N>& e) requires(isLambdaExpr<N>) 
    {return e.next.template run<O::cnt-1>(e.op);}

  template<typename O,typename N>
  const auto beta(const Expr<O,N>& e) requires(!isLambdaExpr<N>) 
    {return e.op.beta(e.next);}

  const auto beta(const auto o) {cout<<"•";return o;}

};

namespace Combinators {
  struct I:Combinator<I,1> {
    static cex const auto& beta(const auto& o) {return o;}
  };cex const I id;
  
  struct K:Combinator<K,2> {
    static cex const auto& beta(const auto& o,const auto&) {return o;}
  };cex const K _true;
  
  template<typename Out> Out& operator<<(Out& out,const I&) {return out<<"id";}
  template<typename Out> Out& operator<<(Out& out,const K&) {return out<<"true";}

};


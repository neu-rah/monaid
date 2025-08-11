#include <iostream>
#include <concepts>
#include <type_traits>
using namespace std;

#define cex constexpr

template<typename F,typename S> struct Pair {const F fst;const S snd;};

template<typename P,typename N> struct App;

template<typename P,typename N>
cex auto& beta(const App<P,N>& o) {return o.beta();}
cex auto& beta(const auto& o) {return o;}

struct Nil{};
struct Lambda {};
struct LambdaExpr {};
template<typename O> static cex bool isExpr=is_convertible_v<O,LambdaExpr>;
template<typename O> static cex bool isLambda=is_convertible_v<O,Lambda>;

// template<typename O,typename... OO> struct Prog {};
template<typename P,typename N>
struct App:Pair<P&,N>,LambdaExpr {
  using Base=Pair<P&,N>;
  using Base::fst;
  using Base::snd;
  cex auto operator()(const auto& o) const 
  requires(is_invocable_v<N,int>)
    {return App<P,decltype(snd(o))>{fst,snd(o)};}
  cex auto operator()(const auto& o) const 
  requires(!is_invocable_v<N,int>)
    {return App<P,App<N,decltype(o)>>{fst,{snd,o}};}
  
  cex const auto& beta() const requires(!isExpr<N>) {
    if cex(isLambda<P>) return fst.beta(snd);
    else return *this;
  }
  cex const auto beta() const requires( isExpr<N>) {return snd.template run<P::cnt-1>(fst);}

  template<int n>
  cex const auto run(const auto f,const auto&... pp) const
    requires(n>0&&isExpr<N>) {return snd.template run<n-1>(f,pp...,fst);}

  // template<int n>
  // cex const auto run(const auto f,const auto&... pp) const
  //   requires(n==1&&!isExpr<N>) {return f.beta(pp...,fst,snd);}

  template<int n>
  cex const auto run(const auto f,const auto&... pp) const requires(n==0) {
    if cex(isExpr<N>)
      return snd.template run<remove_reference_t<decltype(f.beta(pp...,fst))>::cnt-1>(f.beta(pp...,fst));
    else return ::beta(App<decltype(f.beta(pp...,fst)),N>{f.beta(pp...,fst),snd});
  }
};

namespace combinators {
  template<typename C,int n>
  struct Combinator:Lambda {
    static cex const int cnt=n;
    static cex const C combinator{};
    static cex auto operator()(const auto& o) {return App<const C,const decltype(o)>{combinator,o};}
  };

  struct I:Combinator<I,1> {static cex const auto& beta(const auto& o) {return o;}}; cex const I id;
  struct K:Combinator<K,2> {static cex const auto& beta(const auto& o,const auto&) {return o;}}; cex const K _true;
}; using namespace combinators;

namespace printing {
  using namespace combinators;
  template<typename Out> Out& operator<<(Out& out,const I) {return out<<"id";}
  template<typename Out> Out& operator<<(Out& out,const K) {return out<<"true";}

  template<typename Out,typename P,typename N>
  Out& operator<<(Out& out,const App<P,N>& o) {
    if constexpr (isExpr<N>) return out<<(void*)&o<<"@{"<<&o.fst<<"@"<<o.fst<<" "<<o.snd<<"}";
    else return out<<(void*)&o<<"@{"<<&o.fst<<"@"<<o.fst<<" "<<&o.snd<<"@"<<o.snd<<"}";
  }
}; using namespace printing;

cex const auto ok="ok";
cex const auto fail="fail";
cex const auto zzz="zZz";

int main() {
  auto e0=id(ok)(zzz);
  auto e1=_true(ok)(fail)(zzz);
  auto e2=_true(id)(fail)(ok)(zzz);
  cout<<&ok<<"@"<<ok<<endl;
  cout<<&fail<<"@"<<fail<<endl;
  cout<<&zzz<<"@"<<zzz<<endl;
  cout<<e0<<" => "<<e0.beta()<<endl;
  cout<<e1<<" => "<<e1.beta()<<endl;
  cout<<e2<<" => "<<e2.beta()<<endl;
  return 0;
}
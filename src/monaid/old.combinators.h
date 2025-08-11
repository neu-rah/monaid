#pragma once

#include "monaid/lazy.h"

template<typename C,int n>
struct Combinator {
  static cex const int cnt{n};
  static cex const auto operator()(const auto... oo){
    const auto f{[oo...](const auto... pp){return C::beta(oo...,pp...);}};
    return Expr<n-sizeof...(oo),decltype(f)>{f};
  }
};

struct I:Combinator<I,1> {static cex const auto& beta(const auto& o) {return o;}}; cex const I _I;
struct K:Combinator<K,2> {static cex const auto& beta(const auto& o,const auto&) {return o;}}; cex const K _K;
struct S:Combinator<S,3> {static cex const auto& beta(const auto& f,const auto& g,const auto& o) {return f(o)(g(o));}}; cex const S _S;
struct B:Combinator<B,3> {static cex const auto& beta(const auto& f,const auto& g,const auto& o) {return f(g(o));}}; cex const B _B;
struct C:Combinator<C,3> {static cex const auto& beta(const auto& f,const auto& a,const auto& b) {return f(b)(a);}}; cex const C _C;
struct W:Combinator<W,2> {static cex const auto& beta(const auto& o,const auto& p) {return o(p)(p);}}; cex const W _W;
struct T:Combinator<T,2> {static cex const auto& beta(const auto& o,const auto& f) {return f(o);}}; cex const T _T;
struct V:Combinator<V,3> {static cex const auto& beta(const auto& a,const auto& b,const auto& f) {return f(a)(b);}}; cex const V _V;
struct M:Combinator<M,1> {static cex const auto& beta(const auto& f) {return f(f);}}; cex const M _M;

cex const auto id{_I};
cex const auto _true{_K};
cex const auto _false{_true(id)};
cex const auto _const{_K};
cex const auto flip{_C};
cex const auto _pair{_V};
cex const auto _L{_C(_B)(_M)};
cex const auto _Y{_S(_L)(_L)};
cex const auto _Bb{_B(_B)(_B)};
cex const auto fst{_T(_K)};
cex const auto snd{_T(_false)};

// //bool--
// cex const auto _not{_C};
// cex const auto _or{_M};
// struct And:Combinator<And,2> {static cex auto& beta(const auto& p,const auto& q) {return p(q)(p);}};cex const And _and;
// struct BEq:Combinator<BEq,2> {static cex auto beta(const auto& p,const auto& q) {return p(q)(_not(p));}};cex const BEq beq;
// cex bool toBool(const auto o) {return beta(o(true)(false));}
// struct FromBool:Combinator<FromBool,3> {static cex auto beta(const auto& b,const auto& o,const auto& p) {return b?o:p;}};cex const FromBool fromBool;

// //peano--
// struct Succ:Combinator<Succ,1> {static cex auto& beta(const auto& o) {return _S(_B)(o);}};cex const Succ succ;
// struct Add:Combinator<Add,2> {static cex auto& beta(const auto& n,const auto& o) {return n(succ)(o);}};cex const Add _add;
// cex const auto mul{_B};
// cex const auto _pow{_T};
// cex const auto n0{_false};
// cex const auto n1{id};
// cex const auto n2{succ(n1)};
// cex const auto n3{succ(n2)};
// cex const auto n4{succ(n3)};
// cex const auto n5{succ(n4)};
// cex const auto n6{succ(n5)};
// cex const auto n7{succ(n6)};
// cex const auto n8{succ(n7)};
// cex const auto n9{succ(n8)};
// struct Is0:Combinator<Is0,1> {static cex auto& beta(const auto& o) {return o(_true(_false))(_true);}};cex const Is0 is0;
// struct Phi:Combinator<Phi,1> {static cex auto& beta(const auto& o) {return _pair(snd(o))(succ(snd(o)));}};
// struct Pred:Combinator<Pred,1> {static cex auto& beta(const auto& n) {return fst(n(Phi())(_pair(n0)(n0)));}};cex const Pred pred;
// struct Sub:Combinator<Sub,2> {static cex auto& beta(const auto& n, const auto&  o) {return o(pred)(n);}}; cex const Sub sub;
// struct LEq:Combinator<LEq,2> {static cex auto& beta(const auto& n,const auto& o) {return is0(sub(n)(o));}};cex const LEq leq;
// struct GEq:Combinator<GEq,2> {static cex auto& beta(const auto& n,const auto& o) {return is0(sub(o)(n));}};cex const GEq geq;
// cex const auto gt{_Bb(_not)(leq)};
// struct Eq:Combinator<Eq,2> {static cex auto& beta(const auto& n,const auto& o) {return _and(leq(n)(o))(leq(o)(n));}};cex const Eq eq;
// cex const auto lt{_C(gt)};
// struct NEq:Combinator<NEq,2> {static cex auto& beta(const auto& n,const auto& o) {return _or(gt(n)(o))(gt(o)(n));}};cex const NEq neq;

// //list--
// cex const auto cons{_V};
// cex const auto nil{_K(_K)};
// cex const auto head{fst};
// cex const auto tail{snd};
// struct Null:Combinator<Null,1> {static cex auto& beta(const auto& o) {return o(_true(_true(_false)));}};cex const Null null;
// struct _Length:Combinator<_Length,3> {static cex auto& beta(const auto& f,const auto& cnt,const auto& x) {return null(x)(cnt)(f(succ(cnt))(tail(x)));}};cex const _Length _length;
// cex const auto length{_Y(_length)(n0)};
// struct Drop:Combinator<Drop,2> {static cex auto& beta(const auto& n,const auto& o){return n(tail)(o);}};cex const Drop drop;
// struct Index:Combinator<Index,2> {static cex auto& beta(const auto& x,const auto& n){return head(n(tail)(x));}};cex const Index index;
// struct _Last:Combinator<_Last,2> {static cex auto& beta(const auto& f, const auto& o) {return null(o)(nil)(null(tail(o))(head(o))(f(tail(o))));}};
// cex const auto last{_Y(_Last{})};
// struct _Concat:Combinator<_Concat,3> {static cex auto& beta(const auto& f,const auto& a,const auto& b) {return null(a)(b)(cons(head(a))(f(tail(a))(b)));}};
// cex const auto concat{_Y(_Concat{})};
// struct _Init:Combinator<_Init,2> {static cex auto& beta(const auto& f, const auto& o) {return null(o)(nil)(null(tail(o))(nil)(cons(head(o))(f(tail(o)))));}};
// cex const auto _init{_Y(_Init{})};
// struct _Reverse:Combinator<_Reverse,3> {static cex auto& beta(const auto& f,const auto& a,const auto& l) {return null(l)(a)(f(cons(head(l))(a))(tail(l)));}};
// cex const auto _reverse{_Y(_Reverse{})(nil)};
// struct _TakeR:Combinator<_TakeR,4> {static cex auto& beta(const auto& f,const auto& to,const auto& n, const auto& from) {return is0(n)(to)(f(_pair(head(from))(to))(pred(n))(tail(from)));}};
// cex const auto taker{_Y(_TakeR{})(nil)};
// struct Take:Combinator<Take,2> {static cex auto& beta(const auto& n, const auto& o) {return _reverse(taker(n)(o));}};cex const Take take;
// struct _Nats:Combinator<_Nats,2> {static cex auto&  beta(const auto& f,const auto& n){return cons(n)(f(succ(n)));}};
// cex const auto natsn{_Y(_Nats{})};
// cex const auto nats{natsn(n1)};
// struct Range:Combinator<Range,2> {static cex auto& beta(const auto& s, const auto& e) {return take(sub(e)(s))(natsn(s));}};cex const Range range;
// struct _Map:Combinator<_Map,3> {static cex auto& beta(const auto& g,const auto& f,const auto& o){return null(o)(nil)(cons(f(head(o)))(g(f)(tail(o))));}};
// cex const auto _map{_Y(_Map{})};
// struct _Filter:Combinator<_Filter,3> {static cex auto& beta(const auto& g, const auto& f, const auto& o){return null(o)(nil)(f(head(o))(cons(head(o)))(id)(g(f)(tail(o))));}};
// cex const auto filter{_Y(_Filter{})};






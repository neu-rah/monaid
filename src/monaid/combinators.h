#pragma once

#include "monaid/lazy.h"

//combinators--
cex const auto _I{expr<1,[](const auto& o){return o;}>()};
cex const auto _K{expr<2,[](const auto& o,const auto&){return o;}>()};
cex const auto _S{expr<3,[](const auto& f,const auto& g,const auto& o) {return f(o)(g(o));}>()};
cex const auto _B{expr<3,[](const auto& f,const auto& g,const auto& o) {return f(g(o));}>()};
cex const auto _C{expr<3,[](const auto& f,const auto& a,const auto& b) {return f(b)(a);}>()};
cex const auto _W{expr<2,[](const auto& o,const auto& p) {return o(p)(p);}>()};
cex const auto _T{expr<2,[](const auto& o,const auto& f) {return f(o);}>()};
cex const auto _V{expr<3,[](const auto& a,const auto& b,const auto& f) {return f(a)(b);}>()};
cex const auto _M{expr<1,[](const auto& f) {return f(f);}>()};

cex const auto _KI{_K(_I)};
cex const auto _L{_C(_B)(_M)};
cex const auto _Y{_S(_L)(_L)};
cex const auto _Bb{_B(_B)(_B)};

//language base --
cex const auto id{_I};
cex const auto _true{_K};
cex const auto _false{_true(id)};
cex const auto _const{_K};
cex const auto flip{_C};
cex const auto _pair{_V};
cex const auto fst{_T(_K)};
cex const auto snd{_T(_false)};

//bool--
cex const auto _not{_C};
cex const auto _or{_M};
cex const auto _and{expr<[](const auto& p,const auto& q) {return p(q)(p);}>()};
cex const auto beq{expr<[](const auto& p,const auto& q) {return p(q)(_not(p));}>()};
cex bool toBool(const auto o) {return beta(o(true)(false));}
cex const auto fromBool{expr<3,[](bool b,const auto& o,const auto& p) {return b?o:p;}>()};

//peano--
cex const auto succ{expr<2,[](const auto& n,const auto& f){return _S(_B,n)(f);}>()};
// cex const auto succ{_S(_B)};
cex const auto _add{expr<[](const auto& n,const auto& o) {return n(succ)(o);}>()};
cex const auto mul{_B};
cex const auto _pow{_T};
cex const auto n0{_false};
cex const auto n1{id};
cex const auto n2{succ(n1)};
cex const auto n3{succ(n2)};
cex const auto n4{succ(n3)};
cex const auto n5{succ(n4)};
cex const auto n6{succ(n5)};
cex const auto n7{succ(n6)};
cex const auto n8{succ(n7)};
cex const auto n9{succ(n8)};
cex const auto is0{expr<[](const auto& o) {return o(_true(_false))(_true);}>()};
cex const auto _phi{expr<[](const auto& o) {return _pair(snd(o))(succ(snd(o)));}>()};
cex const auto pred{expr<[](const auto& n) {return fst(n(_phi)(_pair(n0)(n0)));}>()};
cex const auto sub {expr<[](const auto& n, const auto&  o) {return o(pred)(n);}>()};
cex const auto leq {expr<[](const auto& n,const auto& o) {return is0(sub(n)(o));}>()};
cex const auto geq{expr<[](const auto& n,const auto& o) {return is0(sub(o)(n));}>()};
cex const auto gt{_Bb(_not)(leq)};
cex const auto eq{expr<[](const auto& n,const auto& o) {return _and(leq(n)(o))(leq(o)(n));}>()};
cex const auto lt{_C(gt)};
cex const auto neq{expr<2,[](const auto& n,const auto& o) {return _or(gt(n)(o))(gt(o)(n));}>()};

//list--
cex const auto cons{_V};
cex const auto nil{_K(_K)};
cex const auto head{fst};
cex const auto tail{snd};
cex const auto null{expr<[](const auto& o) {return o(_true(_true(_false)));}>()};
cex const auto length{_Y(expr<3,[](const auto& f,const auto& cnt,const auto& x) {return null(x)(cnt)(f(succ(cnt))(tail(x)));}>())(n0)};
cex const auto drop{expr<[](const auto& n,const auto& o){return n(tail)(o);}>()};
cex const auto index{expr<[](const auto& x,const auto& n){return head(n(tail)(x));}>()};
cex const auto last{_Y(expr<[](const auto& f, const auto& o) {return null(o)(nil)(null(tail(o))(head(o))(f(tail(o))));}>())};
cex const auto concat{_Y(expr<[](const auto& f,const auto& a,const auto& b) {return null(a)(b)(cons(head(a))(f(tail(a))(b)));}>())};
cex const auto _init{_Y(expr<[](const auto& f, const auto& o) {return null(o)(nil)(null(tail(o))(nil)(cons(head(o))(f(tail(o)))));}>())};
cex const auto _reverse{_Y(expr<[](const auto& f,const auto& a,const auto& l) {return null(l)(a)(f(cons(head(l))(a))(tail(l)));}>())(nil)};
cex const auto taker{_Y(expr<[](const auto& f,const auto& to,const auto& n, const auto& from) {return is0(n)(to)(f(_pair(head(from))(to))(pred(n))(tail(from)));}>())(nil)};
cex const auto take{expr<2,[](const auto& n, const auto& o) {return _reverse(taker(n)(o));}>()};
cex const auto natsn{_Y(expr<2,[](const auto& f,const auto& n){return cons(n)(f(succ(n)));}>())};
cex const auto nats{natsn(n1)};
cex const auto range{expr<[](const auto& s, const auto& e) {return take(sub(e)(s))(natsn(s));}>()};
cex const auto _map{_Y(expr<[](const auto& g,const auto& f,const auto& o){return null(o)(nil)(cons(f(head(o)))(g(f)(tail(o))));}>())};
cex const auto filter{_Y(expr<3,[](const auto& g, const auto& f, const auto& o){return null(o)(nil)(f(head(o))(cons(head(o)))(id)(g(f)(tail(o))));}>())};



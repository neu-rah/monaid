#pragma once

#include "monaid/monad.h"
#include "monaid/monoid.h"

#include <cassert>

template<typename O=Empty>
struct Maybe:Monad<Maybe>,Monoid {
  using Type=O;
  Type m_value{};
  bool m_isJust{false};
  constexpr Maybe(){}
  constexpr Maybe(O o) requires(!std::is_same_v<O,Empty>):m_value{o},m_isJust{true}{}
  constexpr Maybe(const Empty&):m_value{}{}
  constexpr Maybe(const Param<O>& o):m_value{o.m_param},m_isJust{true}{}
  template<typename K>
  requires(std::is_same_v<O,Empty>)
  operator Maybe<K>() const {return {K(m_value)};}
  using Monad<Maybe>::fmap;
  auto fmap(const auto& f) const {
    if(m_isJust) return Maybe<decltype(f(m_value))>(f(m_value));
    else return Maybe<decltype(f(m_value))>();
  }
  auto bind(const auto& f) const {
    // auto x=f(m_value);
    if(m_isJust) return Maybe<decltype(f(m_value))>(f(m_value));
    else return Maybe<decltype(f(m_value))>{};
  }
  template<typename F> auto operator>>(const F f) const {return bind(f);};
  auto liftM2(const auto& f,auto&& mb) {
    if(m_isJust&&mb.m_isJust)
      return Maybe<decltype(f(m_value,mb.m_value))>
        (f(fw$(m_value),fw$(mb.m_value)));
    else return Maybe<decltype(f(m_value,mb.m_value))>();
  }
  auto mappend(Maybe<O>& o) {
    Type empty;
    Type& a=m_isJust?m_value:empty;
    Type& b=o.m_isJust?o.m_value:empty;
    return ::mappend(a,b);
  }
  template<typename Out>
  Out& operator<<(Out& out) const
    {return m_isJust?out<<"Just "<<m_value:out<<"Nothing";}
};

template<typename O> struct Just:Maybe<O>{
  // using Maybe<O>::Maybe;
  Just(O o):Maybe<O>{o}{}
};
template<typename O> struct Nothing:Maybe<O>{
  Nothing():Maybe<O>{}{}
  // using Maybe<O>::Maybe;
};

bool isJust(Maybe<auto> o) {return o.m_isJust;}
bool isNothing(Maybe<auto> o) {return !isJust(o);}
auto fromJust(Maybe<auto> o) {assert(o.m_isJust);return o.m_value;}

template<typename Out,typename O>
Out& operator<<(Out& out,const Maybe<O>& o) {return o.operator<<(out);}
template<typename Out,typename O>
Out& operator<<(Out& out,const Just<O>& o) {return o.operator<<(out);}
template<typename Out,typename O>
Out& operator<<(Out& out,const Nothing<O>& o) {return o.operator<<(out);}


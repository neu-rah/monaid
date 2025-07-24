#pragma once

#include "monaid/monad.h"
#include "monaid/monoid.h"

#include <functional>

template<typename L,typename R>
struct Either:Monad<Either>,Monoid {
  using Left=L;
  using Right=R;
  using Type=R;
  union {
    Left m_left;
    Right m_right;
  };
  bool m_isRight{false};
  Either(Right r,bool ir=true):m_right{r},m_isRight{ir}{}
  Either(Left l,bool ir=false) requires(!std::is_same_v<R,L>):m_left{l},m_isRight{ir}{}
  Either(const Either& o):m_isRight{o.m_isRight} {
    if(m_isRight) m_right=o.m_right;
    else m_left=o.m_left;
  }
  ~Either(){}
  template<typename K>
  requires(std::is_same_v<R,Empty>)
  operator Maybe<K>() const {return {K(m_right)};}
  using Monad<Either>::fmap;
  auto fmap(const auto& f) const {
    if(m_isRight) return Either<L,decltype(f(m_right))>(f(m_right));
    else return Either<L,decltype(f(m_right))>(m_left);
  }
  auto bind(const auto& f) const requires(!std::is_same_v<R,Empty>) {
    if(m_isRight) return Either<L,typename decltype(f(m_right))::Type>(f(m_right));
    else return Either<L,typename decltype(f(m_right))::Type>{m_left};
  }
  auto bind(const auto& f) requires(std::is_same_v<R,Empty>) 
    {return *this;}
  template<typename F> auto operator>>(const F f) const {return bind(f);};
  auto liftM2(const auto& f,auto&& mb) {
    if(m_isRight&&mb.m_isRight)
      return Either<L,decltype(f(m_right,mb.m_right))>
        (f(fw$(m_right),fw$(mb.m_right)));
    else return Either<L,decltype(f(m_right,mb.m_right))>(m_left);
  }
  auto mappend(Either<L,R>& o) {
    if(m_isRight) {
      if(o.m_isRight) return Either<L,R>{::mappend(m_right,o.m_right)};
      else return *this;
    } else return o;
  }
};

template<typename L,typename R>
struct Left:Either<L,R> {Left(L l):Either<L,R>{l,false}{}};

template<typename L,typename R>
struct Right:Either<L,R> { Right(R r):Either<L,R>{r,true}{}};

bool isRight(const Either<auto,auto> o) {return o.m_isRight;}
bool isLeft(const Either<auto,auto> o) {return !isRight(o);}
auto fromRight(const Either<auto,auto> o) {assert(o.m_isRight);return o.m_right;}
auto fromLeft(const Either<auto,auto> o) {assert(!o.m_isRight);return o.m_left;}

template<typename Out,typename L,typename R>
Out& operator<<(Out& out,const Either<L,R> o) 
  {return o.m_isRight?out<<"Right "<<o.m_right:out<<"Left "<<o.m_left;}


#pragma once

#include "monad.h"
#include "monoid.h"
#include <functional>

template<typename R,typename L>
struct Either:Monad<Either>,Monoid {
  using Left=L;
  using Right=R;
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
    if(m_isRight) return Either<decltype(f(m_right)),L>(f(m_right));
    else return Either<decltype(f(m_right)),L>(m_left);
  }
  auto bind(const auto& f) const {
    if(m_isRight) return Either<decltype(f(m_right)),L>(f(m_right));
    else return Either<decltype(f(m_right)),L>{m_left};
  }
  auto liftM2(const auto& f,auto&& mb) {
    if(m_isRight&&mb.m_isRight)
      return Either<decltype(f(m_right,mb.m_right)),L>
        (f(fw$(m_right),fw$(mb.m_right)));
    else return Either<decltype(f(m_right,mb.m_right)),L>(m_left);
  }
  auto mappend(Either<R,L>& o) {
    if(m_isRight) {
      if(o.m_isRight) return Either<R,L>{::mappend(m_right,o.m_right)};
      else return *this;
    } else return o;
  }
};

template<typename R,typename L>
struct Left:Either<R,L> {Left(L l):Either<R,L>{l,false}{}};

template<typename R,typename L>
struct Right:Either<R,L> { Right(R r):Either<R,L>{r,true}{}};

bool isRight(const Either<auto,auto> o) {return o.m_isRight;}
bool isLeft(const Either<auto,auto> o) {return !isRight(o);}
auto fromRight(const Either<auto,auto> o) {assert(o.m_isRight);return o.m_right;}
auto fromLeft(const Either<auto,auto> o) {assert(!o.m_isRight);return o.m_left;}

template<typename Out,typename R,typename L>
Out& operator<<(Out& out,const Either<R,L> o) 
  {return o.m_isRight?out<<"Right "<<o.m_right:out<<"Left "<<o.m_left;}


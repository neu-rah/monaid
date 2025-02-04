#pragma once

#include "monaid/base.h"

constexpr const auto pure(auto o){return Param(fw$(o));}//Monad

//fmap--
auto __fmap(const auto& f,aFunctor auto&& m)
  {return fw$(m).fmap(f);}

const auto fmap=$([](const auto& f,auto&& m){return _fmap(f,fw$(m));});//Functor

//bind--
const auto __bind(const auto& f,auto&& m)
  {return fw$(m).bind(f);};

const auto mbind=$([](const auto& f,auto&& m)
  {return _bind(f,fw$(m));});

//liftM2--
auto __liftM2(const auto& f,auto&& ma,auto&& mb)
  {return fw$(ma).liftM2(f,fw$(mb));};

const auto liftM2=$([](const auto& f,auto&& ma,auto&& mb)
  {return fw$(ma).liftM2(f,fw$(mb));});

//type classes -----------------------
template<template<typename...> class M>
struct Functor {
  auto fmap(const auto& f,auto m) const {return m.fmap(f);}
  auto fmap(const auto& f) const {static_assert(false,"implement fmap :: (a -> b) -> f a -> f b");}
};
template<template<typename...> class M> struct Applicative:Functor<M> {
  //liftM2 :: (a -> b -> c) -> f a -> f b -> f c
  auto liftM2(auto f,auto ma,auto mb) const {return ma.liftM2(f,mb);}
};
template<template<typename...> class M> struct Monad:Applicative<M> {};

// template<aMonad M,typename F> auto operator>>(const M m,const F f) {return m.bind(f);};


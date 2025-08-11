#pragma once

#include <functional>
#include <type_traits>

#define cex constexpr

#define fw$$(oo) std::forward<decltype(oo)>(oo)...
#define fw$(o) std::forward<decltype(o)>(o)
#define t$(o) decltype(o)
#define nrt$(o) std::remove_reference_t<decltype(o)>

template<bool chk,typename T=void>
using When=typename std::enable_if<chk,T>::type;

struct Nil{};
template<typename Out> Out& operator<<(Out& out,const Nil&) {return out<<"Nil";}

template<typename> struct Param;
// template<typename O=Empty> struct Maybe;

#ifdef DEBUG
  #include <iostream>
  using std::cout;
  using std::endl;
#endif

//concepts--
template<typename T> concept aString
  =requires(T o) { {o.starts_with("")}->std::same_as<bool>; };
template<typename O> concept aMonoid
  =requires(O o) { {o.mappend(O{})}->std::same_as<O&>; };
template<typename O> concept aFunctor
  =requires(O o) { {o.fmap([](auto o){return o;}) }; };
template<typename O> concept aMonad
  =requires(O o) { {o.bind([](auto o){return o;}) }; };
template<typename O> concept aFoldable
  =requires(O o) { {o.foldr([](auto a,auto b){return b;},typename O::value_type{})}; };
template<typename O> concept aContainer
  =requires(O o){ { o.begin() } -> std::same_as<decltype(o.begin())>; };
template<typename T> concept aNakedContainer
  =(aContainer<T> && (!(aMonoid<T>||aFunctor<T>)));

template<typename O> concept Code = (!std::is_constructible_v<O>);

auto& operator<<(auto& out,const Code auto&) {return out<<"λ";}

auto _mappend(auto& a,auto& b);
auto _mappend(auto&& a,auto&& b);

auto _foldr(const auto& f,auto&& o,auto&& mo);
auto _fmap(const auto& f,auto& m);
auto _fmap(const auto& f,auto&& m);

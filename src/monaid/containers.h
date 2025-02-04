/**
 * @file containers.h
 * @author Rui Azevedo (ruihfazevedo@gamil.com)
 * @brief some std containers as Monoids
 * @date 2025-01-30
 * 
 */
#pragma once

#include <numeric>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <iterator>

#include "monaid/base.h"

//print a container--
// template<typename Out,template<typename> class M,typename O,template<O, typename ALLOC=std::allocator<O> > class C>
// When<!is_same_v<M<O>,C>,Out>& operator<<(Out& out,M<O> o) {for (auto i : o) out<<i<<" ";return out;}

auto& operator<<(auto& out,aContainer auto o)
  requires (!aString<decltype(o)>) {// need to relax this rules! we can not predict how many exceptions are outhere
  auto i=o.cbegin();
  out<<"["<<*i++;
  for(;i<o.cend();i++) out<<", "<<*i;
  return out<<"]";
}

auto& operator<<(auto& out,const aString auto& o) {
  // requires (aNakedContainer<nrt$(o)>) {
  using I=typename nrt$(o)::value_type;
  out<<"\"";
  copy(o.begin(),o.end(),std::ostream_iterator<I>(out));
  return out<<"\"";
}

//some sort of empty for containers?
template<aNakedContainer M>
struct Container:Monoid,Monad<Container>,M {
  using M::M;
  auto& mappend(auto&& o) {
    std::move(fw$(o).begin(),fw$(o).end(),std::back_inserter(*this));
    return *this;
  }
  auto foldr(auto&& f,auto&& o)
    {return accumulate(M::begin(), M::end(), fw$(o), fw$(f));};
  auto fmap(auto&& f) {
    M res{};
    transform(M::begin(),M::end(),back_inserter(res),f);
    return res;
  }
  operator M()=delete;
};

auto __mappend(aNakedContainer auto&& a,aContainer auto&& b) {
  using O=std::remove_reference_t<decltype(a)>;
  Container<O> o;
  o.mappend(fw$(a));
  o.mappend(fw$(b));
  return o;
};

template<typename O>
auto __mappend(Container<O>& a,aContainer auto& b) {
  // requires(!aNakedContainer<t$(a)>){
  // using O=typename nrt$(a);
  Container<O> o;
  o.mappend(fw$(a));
  o.mappend(fw$(b));
  return o;
};

auto __foldr(const auto& f,auto&& o,aContainer auto&& mo)
  {return mo.foldr(f,fw$(o));}

auto __foldr(const auto& f,auto&& o,aNakedContainer auto&& mo)
  {return accumulate(fw$(mo).begin(), fw$(mo).end(), fw$(o), f);};


auto __fmap(auto&& f,aContainer auto mo) {//->remove_reference_t<decltype(mo)> {
  using CT=nrt$(mo);
  CT res{};
  transform(begin(mo),end(mo),back_inserter(res),f);
  return res;
}
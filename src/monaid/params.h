#pragma once

struct Empty {
  using Type=Empty;
  template<typename K> constexpr operator K() const {return K();}
  auto fmap(const auto&) const {return *this;}
  auto bind(const auto&) const {return *this;}
};
template<typename Out> Out& operator<<(Out& out,const Empty&) {return out<<"ø";}

template<typename O>
struct Param {
  using Type=O;
  Type m_param;
  template<typename K> constexpr operator K() const {return K(m_param);}
  auto fmap(const auto& f) const {return Param<decltype(f(m_param))>(f(m_param));}
  auto bind(const auto& f) const {return f(m_param);}
};
template<typename Out,typename O>
Out& operator<<(Out& out,const Param<O>& o)
  {return out<<"{"<<o.m_param<<"}";}


auto __mappend(const Empty&,auto&& o) {return o;};
auto __mappend(auto&& o,const Empty&) {return o;};
inline auto __mappend(const Empty& o,const Empty&) {return o;};

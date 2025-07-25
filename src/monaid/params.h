#pragma once

/// @brief holds an abstract empty "value" still waiting for its final type
///   this is the support of mempty
struct Empty {
  using Type=Empty;
  /// @brief converts to anything
  /// @tparam K the anything type
  template<typename K> constexpr operator K() const {return K();}
  auto fmap(const auto&) const {return *this;}
  auto bind(const auto&) const {return *this;}
};
template<typename Out> Out& operator<<(Out& out,const Empty&) {return out<<"ø";}

/// @brief holds a monadic value still waiting for the final type, it can then convert to Either or Maybe, ...
///   this is the (data transport) support for `pure` implementation
/// @tparam O value type
template<typename O>
struct Param {
  using Type=O;
  Type m_param;
  template<typename K> constexpr operator K() const {return K(m_param);}
  auto fmap(const auto& f) const {return Param<decltype(f(m_param))>(f(m_param));}
  auto bind(const auto& f) const {return f(m_param);}
};

/// @brief print the transient data
template<typename Out,typename O>
Out& operator<<(Out& out,const Param<O>& o)
  {return out<<"{"<<o.m_param<<"}";}


auto __mappend(const Empty&,auto&& o) {return o;};
auto __mappend(auto&& o,const Empty&) {return o;};
inline auto __mappend(const Empty& o,const Empty&) {return o;};

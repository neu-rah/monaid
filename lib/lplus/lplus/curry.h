#pragma once
#include <functional>
#include <utility>
#include "lplus/base.h"

// curry --
constexpr const auto $(const auto&& f,auto&&... oo)
  requires std::is_invocable_v<decltype(f),decltype(oo)...> 
  {return std::invoke(fw$(f),fw$$(oo));}

constexpr const auto $(const auto&& f,auto&&... oo)
  requires(!std::is_invocable_v<decltype(f),decltype(oo)...>)
  {return [&f,&oo...](auto&&... pp){return $(fw$(f),fw$$(oo),fw$$(pp));};}


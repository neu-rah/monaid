#pragma once

#include <concepts>

#include "lplus/curry.h"
#include "lplus/params.h"

constexpr const auto mempty{Empty{}};//Monoid

struct Neutral {//pedantic
  //* mempty :: a
  static constexpr const auto mempty{::mempty};
};
struct Semigroup:Neutral {//very pedantic
  //sconcat :: NonEmpty a -> a
};
struct Monoid:Semigroup {
  // (<>) == mappend
  //* mappend :: a -> a -> a
  // mappend [1,2] [3,4] => [1,2,3,4]
  auto mappend(auto o) {static_assert(false,"implement mappend :: a -> a -> a");}

  // mconcat :: [a] -> a
  // mconcat ["1","2","345"] -> "12345"
};

auto __mappend(auto&& a,auto&& b) requires(!aNakedContainer<decltype(a)>) {return fw$(a).mappend(fw$(b));};

const auto mappend=$([](auto&& a,auto&& b)
  {return _mappend(fw$(a),fw$(b));});

auto operator+(auto&& a,auto&& b){return mappend(fw$(a),fw$(b));}

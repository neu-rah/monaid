#pragma once

#include "lplus/curry.h"

struct Foldable {
  // foldMap :: Monoid m => (a -> m) -> t a -> m 
  // foldr :: (a -> b -> b) -> b -> t a -> b
  auto foldr(auto,auto,auto){static_assert(false,"implement foldr :: (a -> b -> b) -> b -> t a -> b");}
};

const auto foldr=$([](const auto& f,auto&& o,auto&& mo)
  {return _foldr(f,fw$(o),fw$(mo));});
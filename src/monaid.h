#pragma once

#include "monaid/base.h"
#include "monaid/curry.h"
#include "monaid/params.h"
#include "monaid/monoid.h"
#include "monaid/monad.h"
#include "monaid/containers.h"
#include "monaid/maybe.h"
#include "monaid/either.h"
#include "monaid/foldable.h"

auto _mappend(auto& a,auto& b) {return __mappend(a,b);};
auto _mappend(auto&& a,auto&& b) {return __mappend(fw$(a),fw$(b));};
auto _foldr(const auto& f,auto&& o,auto&& mo) {return __foldr(f,fw$(o),fw$(mo));}
auto _fmap(const auto& f,auto& m) {return m.fmap(f);}
auto _fmap(const auto& f,auto&& m) {return fw$(m).fmap(f);}
auto _bind(const auto& f,auto&& m) {return __bind(f,fw$(m));};
auto _liftM2(const auto& f,auto&& ma,auto&& mb) {return _liftM2(f,fw$(ma),fw$(mb));};

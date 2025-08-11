#include <iostream>
using namespace std;

#include "combinator.h"
using namespace Combinators;
using namespace Lazy;

namespace printing {//-------------------
  template<typename Out,typename O>
  Out& operator<<(Out& out,const Prog<O>& o)
    {return out<<o.head;}
  template<typename Out,typename O,typename... OO>
  Out& operator<<(Out& out,const Prog<O,OO...>& o)
    {return out<<o.head<<" "<<o.tail;}
  template<typename Out,typename O,typename N>
  Out& operator<<(Out& out,const Expr<O,N>& o)
    {return out<<o.op<<" "<<o.next;}
}; using namespace printing;
  
constexpr const char* ok="ok";
constexpr const char* fail="fail";
constexpr const char* zzz="zZz";

namespace test {
  cex Prog id{::id,ok};
  cex Prog _true{::_true,ok,fail};
  cex Prog _false{::_true,::id,fail,ok};
};

int main() {
  // cout<<test::id<<" => "<<beta(test::id)<<endl;
  // cout<<test::_true<<" => "<<beta(test::_true)<<endl;
  // cout<<test::_false<<" => "<<beta(test::_false)<<endl;
  // auto e0=id(1967);
  // auto e1=_true(ok)(fail);
  auto e2=_true(id)(fail)(ok);
  // cout<<e0<<" -> "<<beta(e0)<<endl;
  // cout<<e1<<" -> "<<beta(e1)<<endl;
  cout<<e2<<" -> "; 
  cout<<beta(e2)<<endl;
  return 0;
}
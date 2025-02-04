#include <monaid.h>
#include <iostream>
using namespace std;

//library tests are also included in this example (folder /test)

auto _const{$([](auto o,auto) {return o;})};

int main() {
  cout<<"Monad bind"<<endl;
  cout<<"successful computation:"<<(
    Just(2)
      >>( [](auto o) {return pure(2*o);})
      >>( [](auto o) {return pure((double)3.1*o);} )//changing type int->double
  )<<endl;

  cout<<"failing computation:"<<(
    Just(2)
      >>( [](auto o) {return pure(2*o);})
      >>(_const(mempty))//computation that will fail!
      >>( [](auto o) {return pure((double)3.1*o);} )//changing type int->double
  )<<endl;

  cout<<"failing computation with error message:"<<(
    Either<int,const char*>(2)
      >>( [](auto o) {return pure(2*o);})
      >>(_const(Left<int,const char*>("failing, just because.")))//computation that will fail!
      >>( [](auto o) {return pure((double)3.1*o);} )//changing type int->double
  )<<endl;

  cout<<"Functor fmap"<<endl;
  const auto calc_age=$([](auto year,auto born) {return year-born;});//curried lambda
  const auto age=calc_age(2025);//partial application
  cout<<"age:"<<(Just(1967).fmap(age))<<endl;

  cout<<"Folding a standard container"<<endl;
  cout<<"foldr (+) 10 [1,2,3]:"<<(
    foldr([](auto a,auto b){return a+b;})(10)(vector<int>{1,2,3})
  )<<endl;

  cout<<"mappend strings (std container):"<<(mappend(string("ok"),string(", let's go...")))<<endl;


  return 0;
}
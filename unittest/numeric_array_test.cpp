#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <catch.hpp>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("numeric_array") {
  using Shape1 = KShape<3, 10>;
  using Shape2 = KShape<3, 7>;
  using NumericArray1 = NumericArray<double, Shape1>;
  using NumericArray2 = NumericArray<double, Shape2>;
  NumericArray1 array1{Shape1{}};
  NumericArray2 array2{Shape2{}};
  SECTION("arithmetic") {
    auto expr1 = array1 + array1;
    //auto expr2 = array1 * array2; //static assertion failure
  }
}

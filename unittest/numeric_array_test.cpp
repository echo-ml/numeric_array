#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <catch.hpp>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("numeric_array") {
  using Shape1 = KShape<3, 10>;
  using NumericArray1 = NumericArray<double, Shape1>;
  NumericArray1 array1{Shape1{}};
}

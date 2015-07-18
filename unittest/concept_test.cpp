#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/concept.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("like_valued_numeric_array") {
  NumericArray<double, ShapeC<3, 7>> n1;
  NumericArray<double, ShapeC<2, 1, 3>> n2;
  NumericArray<double, ShapeC<4, 4>> n3;
  NumericArray<float, ShapeC<9, 8>> n4;

  CHECK(numeric_array::concept::like_valued_numeric_arrays<decltype(n1),
                                                           decltype(n2)>());
  CHECK(!numeric_array::concept::like_valued_numeric_arrays<decltype(n1),
                                                           decltype(n4)>());
  CHECK(numeric_array::concept::like_valued_numeric_arrays<
      decltype(n1), decltype(n2), decltype(n3)>());
}

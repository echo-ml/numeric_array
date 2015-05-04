#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <echo/numeric_array/concept.h>
#include <catch.hpp>
#include <numeric>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("numeric_array") {
  using Shape1 = KShape<3, 2>;
  using NumericArray1 = NumericArray<double, Shape1>;
  CHECK(numeric_array::concept::numeric_array<NumericArray1>());
  // using Shape2 = KShape<3, 7>;
  // using NumericArray2 = NumericArray<double, Shape2>;
  // NumericArray1 array1{Shape1{}};
  // NumericArray2 array2{Shape2{}};
  // NumericArray1 array3{Shape1{}};
  // std::iota(all_begin(array1), all_end(array1), 0);
  // std::iota(all_begin(array3), all_end(array3), 1);
  //
  // SECTION("arithmetic") {
  //   auto expr1 = array1 + array1;
  //   // auto compilation_failure = array1 * array2; //static assertion failure
  //   auto expr2 = sqrt(array1);
  //
  //   // auto expr3 = pow(array1, array1);
  //
  //   REQUIRE(expr1.evaluator()(Index<1>(0)) == 0);
  //   REQUIRE(expr1.evaluator()(Index<1>(1)) == 2);
  //
  //   REQUIRE(expr2.evaluator()(Index<1>(0)) == 0);
  //   REQUIRE(expr2.evaluator()(Index<1>(1)) == 1);
  //   REQUIRE(expr2.evaluator()(Index<1>(2)) == std::sqrt(2.0));
  // }
  //
  // SECTION("assign/move") {
  //   array3 = array1;
  //   array3 = std::move(array1);
  // }
  //
  // SECTION("expression assignment") {
  //   auto assign1 = (array3 = sqrt(array1));
  //   auto assign2 = (array3 += array1);
  //
  //   REQUIRE(assign1.evaluator()(Index<1>(2)) == std::sqrt(2));
  //   REQUIRE(assign2.evaluator()(Index<1>(1)) == 3);
  // }
  //
  // SECTION("serial evaluator") {
  //   NumericArrayExecuter executer;
  //   auto assign1 = (array3 = sqrt(array1));
  //   executer(NumericArrayExecuter::serial_mode(), assign1);
  //   REQUIRE(assign1.evaluator()(Index<1>(0)) == std::sqrt(0));
  //   REQUIRE(assign1.evaluator()(Index<1>(5)) == std::sqrt(5));
  // }
  //
  // SECTION("parallel evaluator") {
  //   NumericArrayExecuter executer;
  //   auto assign1 = (array3 = sqrt(array1));
  //   executer(NumericArrayExecuter::parallel_mode(), assign1);
  //   REQUIRE(assign1.evaluator()(Index<1>(0)) == std::sqrt(0));
  //   REQUIRE(assign1.evaluator()(Index<1>(5)) == std::sqrt(5));
  // }
}

#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/map_expression.h>
#include <echo/numeric_array/map_index_expression.h>
#include <echo/tbb_expression_executer.h>
#include <catch.hpp>
#include <numeric>

using namespace echo;
using namespace echo::numeric_array;

const echo::execution_context::intel_tbb::ExpressionExecuter executer{};

TEST_CASE("numeric_array") {
  using Shape1 = KShape<3, 2>;
  using NumericArray1 = NumericArray<double, Shape1>;
  CHECK(numeric_array::concept::numeric_array<NumericArray1>());
  using Shape2 = KShape<3, 7>;
  using NumericArray2 = NumericArray<double, Shape2>;
  NumericArray1 array1{Shape1{}};
  NumericArray2 array2{Shape2{}};
  NumericArray1 array3{Shape1{}};
  std::iota(all_begin(array1), all_end(array1), 0);
  std::iota(all_begin(array3), all_end(array3), 1);

  auto array4 = make_numeric_array_view(
      array1.data(),
      make_k_subshape(make_k_shape(2_index), k_array::KShapeStrides<3>()));

  SECTION("arithmetic") {
    auto expr1 = array1 + array1;
    // auto compilation_failure = array1 * array2; //static assertion failure
    auto expr2 = sqrt(array1);

    // auto expr3 = pow(array1, array1);

    REQUIRE(expr1.evaluator()(Index<1>(0)) == 0);
    REQUIRE(expr1.evaluator()(Index<1>(1)) == 2);

    REQUIRE(expr2.evaluator()(Index<1>(0)) == 0);
    REQUIRE(expr2.evaluator()(Index<1>(1)) == 1);
    REQUIRE(expr2.evaluator()(Index<1>(2)) == std::sqrt(2.0));
  }

  SECTION("assign/move") {
    array3 = array1;
    array3 = std::move(array1);
  }

  SECTION("expression assignment") {
    auto assign1 = (array3 = sqrt(array1));
    auto assign2 = (array3 += array1);

    REQUIRE(assign1.evaluator()(Index<1>(2)) == std::sqrt(2));
    REQUIRE(assign2.evaluator()(Index<1>(1)) == 3);
  }

  SECTION("serial evaluator") {
    auto assign1 = (array3 = sqrt(array1));
    executer(assign1);
    REQUIRE(assign1.evaluator()(Index<1>(0)) == std::sqrt(0));
    REQUIRE(assign1.evaluator()(Index<1>(5)) == std::sqrt(5));
  }

  SECTION("subarray") {
    std::fill_n(array1.data(), int(get_num_elements(array1)), 0);
    auto expr =
      array4 = map_index([](index_t i) { return (i+1)*(i+1); }, 2_index);
    executer(expr);

    CHECK(array1(0, 0) == 1);
    CHECK(array1(0, 1) == 4);
  }
}

TEST_CASE("accessor") {
  NumericArray<double, KShape<2,3>> n1;
  const auto& n1_cref = n1;
  n1(0, 1) = 7;
  CHECK(n1_cref(0, 1) == 7);
}

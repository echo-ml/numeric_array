#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/map_expression.h>
#include <echo/test.h>
#include <numeric>
#include <functional>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("expression_test") {
  NumericArray<double, KShape<3,2>> a1;
  auto expr1 = make_expression(numeric_array_expression_tag(), a1);
  auto expr2 = make_expression(numeric_array_expression_tag(), 2.0);
  auto expr3 = make_expression(numeric_array_expression_tag(), 3.0);

  using E1 = decltype(expr1);
  using E2 = decltype(expr2);
  using E3 = decltype(expr3);
  CHECK(numeric_array::concept::compatible_expressions<E1, E2>());
  CHECK(!numeric_array::concept::compatible_expressions<E3, E2>());
  CHECK(numeric_array::concept::compatible_expressions<E3, E1, E2>());
}

TEST_CASE("map_expression") {
  NumericArray<double, KShape<3,2>> a1;
  auto expr1 = make_expression(numeric_array_expression_tag(), a1);
  a1 = {1,2,3,4,5,6};
  auto expr2 = make_expression(numeric_array_expression_tag(), 2.0);

  auto expr3 = make_map_expression(numeric_array_expression_tag(),
    std::plus<double>(), expr1, expr2);

  const auto& eval = expr3.evaluator();
  CHECK(eval(0) == 3.0);
  CHECK(eval(5) == 8.0);
}

TEST_CASE("arithmetic_expression") {
  NumericArray<double, KShape<3,2>> a1, a2;
  a2 = {1,2,3,4,5,6};
  auto e1 = a2*a2;
  auto e2 = (a1 = a2*a2);
  auto eval = e2.evaluator();
  eval(0); eval(5);
  CHECK(a1(0,0) == 1);
  CHECK(a1(2,1) == 36);
  // auto expr = (a1 = a2*a2);
}

#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/map_expression.h>
#include <echo/test.h>
#include <numeric>
#include <functional>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("expression_test") {
  NumericArray<double, ShapeC<3,2>> a1;
  NumericArray<double, ShapeC<2,3>> a2;
  auto expr1 = make_expression(numeric_array_expression_tag(), a1);
  auto expr2 = make_expression(numeric_array_expression_tag(), 2.0);
  auto expr3 = make_expression(numeric_array_expression_tag(), 3.0);
  auto expr4 = make_expression(numeric_array_expression_tag(), a2);

  using E1 = decltype(expr1);
  using E2 = decltype(expr2);
  using E3 = decltype(expr3);
  using E4 = decltype(expr4);
  CHECK(numeric_array::concept::compatible_expressions<E1, E2>());
  CHECK(!numeric_array::concept::compatible_expressions<E3, E2>());
  CHECK(numeric_array::concept::compatible_expressions<E3, E1, E2>());
  CHECK(!numeric_array::concept::compatible_expressions<E3, E1, E4, E2>());
}

TEST_CASE("map_expression") {
  NumericArray<double, ShapeC<3,2>> a1;
  auto expr1 = make_expression(numeric_array_expression_tag(), a1);
  a1 = {{1, 4}, {2, 5}, {3, 6}};
  auto expr2 = make_expression(numeric_array_expression_tag(), 2.0);

  auto expr3 = make_map_expression(numeric_array_expression_tag(),
    std::plus<double>(), expr1, expr2);

  const auto& eval = expr3.evaluator();
  CHECK(eval(0) == 3.0);
  CHECK(eval(5) == 8.0);

  auto expr4 = map(std::plus<double>(), a1, a1);
  auto eval4 = expr4.evaluator();
  CHECK(eval4(0) == 2.0);
  CHECK(eval4(5) == 12.0);
}

TEST_CASE("arithmetic_expression") {
  NumericArray<double, ShapeC<3,2>> a1, a2;
  a2 = {{1, 4}, {2, 5}, {3, 6}};
  auto e1 = a2*a2;
  auto e2 = (a1 = a2*a2);
  auto eval = e2.evaluator();
  eval(0); eval(5);
  CHECK(a1(0,0) == 1);
  CHECK(a1(2,1) == 36);
}

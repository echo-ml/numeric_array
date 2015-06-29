#include <echo/numeric_array/map_indexes_expression.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("map_indexes_expression") {
  auto f1 = [](index_t i) { return i; };
  auto f2 = [](index_t i, index_t j) { return i + 10 * j; };

  auto expr1 = map_indexes(f1, 3);
  auto eval1 = expr1.evaluator();
  CHECK(eval1(0) == 0);
  CHECK(eval1(1) == 1);

  auto expr2 = map_indexes(f2, 3, 2);
  auto eval2 = expr2.evaluator();
  CHECK(eval2(0, 3, 1, 2) == 10);
  CHECK(eval2(1, 3, 0, 2) == 1);

  auto expr3 = map_indexes<execution_context::structure::general>(f1, 3);

  auto expr4 = map_indexes(f1, make_dimensionality(3));
  auto eval4 = expr1.evaluator();
  CHECK(eval4(0) == 0);
  CHECK(eval4(1) == 1);

  auto expr5 = map_indexes<execution_context::structure::general>(
      f2, make_dimensionality(3, 2));
}

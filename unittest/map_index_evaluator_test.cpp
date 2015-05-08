#include <echo/numeric_array/map_index_evaluator.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::numeric_array;

TEST_CASE("map_index_evaluator") {
  auto f1 = [](index_t i) { return i; };
  auto f2 = [](index_t i, index_t j) { return i + j; };

  CHECK(numeric_array::concept::index_functor<1, decltype(f1)>());
  CHECK(!numeric_array::concept::index_functor<2, decltype(f1)>());
  CHECK(!numeric_array::concept::index_functor<2, int>());
  CHECK(numeric_array::concept::index_functor<2, decltype(f2)>());

  auto eval1 = make_map_index_evaluator<1>(f1);
  CHECK(eval1(0) == 0);
  CHECK(eval1(1) == 1);

  auto eval2 = make_map_index_evaluator<2>(f2);
  CHECK(eval2(0, 3, 1, 5) == 1);
  CHECK(eval2(1, 3, 0, 5) == 1);
  CHECK(eval2(2, 3, 1, 5) == 3);
}

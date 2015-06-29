#include <echo/numeric_array/evaluator.h>
#include <echo/numeric_array/scalar_evaluator.h>
#include <echo/numeric_array/flatten_evaluator.h>
#include <echo/numeric_array/numeric_array_evaluator.h>
#include <echo/test.h>
#include <numeric>
#include <functional>

using namespace echo;
using namespace echo::numeric_array;

// TEST_CASE("scalar_evaluator") {
//   auto evaluator = make_scalar_evaluator(2.8);
//   CHECK(evaluator(2) == 2.8);
// }
//
// TEST_CASE("flatten_evaluator") {
//   auto scalar_evaluator = make_scalar_evaluator(3.1);
//   auto flatten_evaluator1 = make_flatten_evaluator<2>(scalar_evaluator);
//   auto flatten_evaluator2 = make_flatten_evaluator<3>(scalar_evaluator);
//
//   CHECK(flatten_evaluator1(0, 0, 0, 0) == 3.1);
//   CHECK(flatten_evaluator2(0, 0, 0, 0, 0, 0) == 3.1);
// }
//
// TEST_CASE("numeric_array_evaluator") {
//   double data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//   auto eval1 = make_numeric_array_evaluator(&data[0]);
//   CHECK(eval1(0) == 1);
//   CHECK(eval1(1) == 2);
// }
//
// TEST_CASE("numeric_subarray_evaluator1") {
//   double data[] = {1, 2, 3, 4};
//   k_array::KShapeStrides<2> shape_strides;
//   auto eval1 = make_numeric_subarray_evaluator(&data[0], shape_strides);
//   CHECK(eval1(0) == 1);
//   CHECK(eval1(1) == 3);
// }
//
// TEST_CASE("numeric_subarray_evaluator2") {
//   double data[] = {1, 2, 3, 4, 5, 6, 7, 8};
//   k_array::KShapeStrides<3, Stride::kDynamic> shape_strides(1);
//   auto eval1 = make_numeric_subarray_evaluator(&data[0], shape_strides);
//   CHECK(eval1(0, 2, 0, 4) == 1);
//   CHECK(eval1(1, 2, 0, 4) == 4);
//   CHECK(eval1(1, 2, 1, 4) == 5);
// }
//
// TEST_CASE("flatten_evaluator2") {
//   double data[2*3*4];
//   std::iota(std::begin(data), std::end(data), 0);
//   auto eval1 = make_numeric_array_evaluator(&data[0]);
//   auto eval2 = make_flatten_evaluator<3>(eval1);
//   CHECK(eval2(0, 2, 0, 3, 0, 4) == 0);
//   CHECK(eval2(1, 2, 0, 3, 0, 4) == 1);
//
//   CHECK(eval2(0, 2, 1, 3, 0, 4) == 2);
//   CHECK(eval2(0, 2, 2, 3, 0, 4) == 4);
//
//   CHECK(eval2(1, 2, 1, 3, 1, 4) == 9);
// }
//
// TEST_CASE("map_evaluator") {
//   auto scalar_evaluator1 = make_flatten_evaluator<2>(make_scalar_evaluator(3.0));
//   auto scalar_evaluator2 = make_flatten_evaluator<2>(make_scalar_evaluator(4.0));
//   auto eval = make_map_evaluator<2>(std::plus<double>(), scalar_evaluator1, scalar_evaluator2);
//   CHECK(eval(0, 0, 0, 0) == 7);
// }
//
// TEST_CASE("map_evaluator2") {
//   double data1[] = {1,2,3,4,5,6};
//   double data2[] = {6,5,4,3,2,1};
//   auto eval1 =
//       make_flatten_evaluator<2>(make_numeric_array_evaluator(&data1[0]));
//   auto eval2 =
//       make_flatten_evaluator<2>(make_numeric_array_evaluator(&data2[0]));
//   
//   auto eval = make_map_evaluator<2>(std::plus<double>(), eval1, eval2);
//   CHECK(eval(0, 2, 0, 3) == 7);
//   CHECK(eval(1, 2, 0, 3) == 7);
// }
//
// TEST_CASE("map_evaluator3") {
//   auto eval1 = make_scalar_evaluator(3.0);
//   double data[2*3];
//   std::iota(std::begin(data), std::end(data), 0);
//   auto eval2 =
//       make_flatten_evaluator<2>(make_numeric_array_evaluator(&data[0]));
//   // auto eval3 = make_map_evaluator<2>(std::plus<double>(), eval1, eval2);
//   // CHECK(eval3(0, 2, 0, 3) == 3);
//   // CHECK(eval3(1, 2, 0, 3) == 4);
//   // CHECK(eval3(1, 2, 1, 3) == 6);
// }

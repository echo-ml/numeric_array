#include <echo/numeric_array.h>
#include <echo/tbb_expression_executer.h>
#include <touchstone/touchstone.h>

using namespace echo;
using namespace echo::numeric_array;

// BENCHMARK_SET("strided_expression1", NumTrials(5), NumEpochs(1),
//               ExpoentialRange(4, 6, 10), Units<std::chrono::microseconds>()) {
//   echo::execution_context::intel_tbb::ExpressionExecuter executer;
//   int N = touchstone::n();
//   int N_div_2 = N / 2;
//   auto rng = touchstone::random_number_generator();
//   std::uniform_real_distribution<double> dist(-1, 1);
//
//   //////////////////////////////
//   // Numeric Array benchmarks //
//   //////////////////////////////
//
//   NumericArray<double, KShape<Dimension::kDynamic>> X1(make_k_shape(N)),
//       Y1(make_k_shape(N_div_2));
//   k_array::KShapeStrides<2> shape_strides;
//   std::fill_n(X1.data(), N, 0);
//   std::fill_n(Y1.data(), N_div_2, 0);
//   auto sub_x1_eval = make_numeric_subarray_evaluator(X1.data(), shape_strides);
//   auto sub_x1_expr =
//       make_numeric_array_expression<execution_context::structure::general>(
//           make_k_shape(N / 2), sub_x1_eval);
//   for (auto&& x : all_range(X1)) x = dist(rng);
//   BENCHMARK("b1") { executer(execution_mode::simd, Y1 = log(sub_x1_expr)); }
//   touchstone::do_not_optimize_away(Y1.data());
//
//   ///////////////////////
//   // vector benchmarks //
//   ///////////////////////
//
//   std::vector<double> X2(N), Y2(N_div_2);
//   for (int i = 0; i < N; ++i) X2[i] = dist(rng);
//   BENCHMARK("b2") {
// #pragma simd
//     for (int i = 0; i < N_div_2; ++i) Y2[i] = std::log(X2[2 * i]);
//   }
//   touchstone::do_not_optimize_away(Y2.data());
// }
//
// BENCHMARK_SET("strided_expression2", NumTrials(5), NumEpochs(1),
//               ExpoentialRange(4, 7, 10), Units<std::chrono::microseconds>()) {
//   echo::execution_context::intel_tbb::ExpressionExecuter executer;
//   int N = touchstone::n();
//   int N_div_2 = N / 2;
//   auto rng = touchstone::random_number_generator();
//   std::uniform_real_distribution<double> dist(-1, 1);
//
//   //////////////////////////////
//   // Numeric Array benchmarks //
//   //////////////////////////////
//
//   NumericArray<double, KShape<Dimension::kDynamic>> X1(make_k_shape(N)),
//       Y1(make_k_shape(N));
//   k_array::KShapeStrides<2> shape_strides;
//   std::fill_n(X1.data(), N, 0);
//   std::fill_n(Y1.data(), N, 0);
//   auto sub_x1_eval = make_numeric_subarray_evaluator(X1.data(), shape_strides);
//   auto sub_y1_eval = make_numeric_subarray_evaluator(Y1.data(), shape_strides);
//   auto sub_x1_expr =
//       make_numeric_array_expression<execution_context::structure::general>(
//           make_k_shape(N / 2), sub_x1_eval);
//   auto sub_y1_expr =
//       make_numeric_array_expression<execution_context::structure::general>(
//           make_k_shape(N / 2), sub_y1_eval);
//
//   for (auto&& x : all_range(X1)) x = dist(rng);
//   BENCHMARK("b1") {
//     executer(execution_mode::simd, sub_y1_expr = sub_x1_expr * sub_x1_expr);
//   }
//   touchstone::do_not_optimize_away(Y1.data());
//
//   ///////////////////////
//   // vector benchmarks //
//   ///////////////////////
//
//   std::vector<double> X2(N), Y2(N);
//   for (int i = 0; i < N; ++i) X2[i] = dist(rng);
//   BENCHMARK("b2") {
// #pragma simd
//     for (int i = 0; i < N_div_2; ++i) Y2[2 * i] = X2[2 * i] * X2[2 * i];
//   }
//   touchstone::do_not_optimize_away(Y2.data());
// }

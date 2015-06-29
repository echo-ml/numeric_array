#include <echo/numeric_array.h>
#include <echo/tbb_expression_executer.h>
#include <touchstone/touchstone.h>

using namespace echo;
using namespace echo::numeric_array;

// BENCHMARK_SET("basic_expression", NumTrials(5), NumEpochs(1),
//   ExpoentialRange(4, 6, 10),
//               Units<std::chrono::microseconds>()) {
//   echo::execution_context::intel_tbb::ExpressionExecuter executer;
//   int N = touchstone::n();
//   auto rng = touchstone::random_number_generator();
//   std::uniform_real_distribution<double> dist(-1, 1);
//
//   //////////////////////////////
//   // Numeric Array benchmarks //
//   //////////////////////////////
//
//   NumericArray<double, KShape<Dimension::kDynamic>> X1(make_k_shape(N)),
//       Y1(make_k_shape(N));
//   std::fill_n(X1.data(), N, 0);
//   std::fill_n(Y1.data(), N, 0);
//   for (auto&& x : all_range(X1)) x = dist(rng);
//   BENCHMARK("b1") { executer(execution_mode::simd, Y1 = X1 * X1); }
//   BENCHMARK("b2") {
//     auto map_expr = make_map_expression(
//         numeric_array_expression_tag(), [](double x) { return x * x; },
//         make_expression(numeric_array_expression_tag(), X1));
//     auto expr = Y1 = map_expr;
//     auto eval = expr.evaluator();
//     executer(execution_mode::simd, Y1 = expr);
//   }
//   touchstone::do_not_optimize_away(Y1.data());
//   BENCHMARK("b3") { executer(execution_mode::simd | execution_mode::nontemporal, Y1 = X1 * X1); }
//
//   ///////////////////////
//   // vector benchmarks //
//   ///////////////////////
//
//   std::vector<double> X2(N), Y2(N);
//   for (int i = 0; i < N; ++i) X2[i] = dist(rng);
//   BENCHMARK("b4") {
// #pragma simd
//     for (int i = 0; i < N; ++i) Y2[i] = X2[i] * X2[i];
//   }
//   BENCHMARK("b5") {
// #pragma simd
// #pragma vector nontemporal
//     for (int i = 0; i < N; ++i) Y2[i] = X2[i] * X2[i];
//   }
//   touchstone::do_not_optimize_away(Y2.data());
// }

#include <echo/numeric_array.h>
#include <echo/tbb_expression_executer.h>
#include <touchstone/touchstone.h>

using namespace echo;
using namespace echo::numeric_array;

BENCHMARK_SET("basic_expression", 
  NumTrials(5), Units<std::chrono::microseconds>())
{
  echo::execution_context::intel_tbb::ExpressionExecuter executer;
  int N = 100'000;
  NumericArray<double, KShape<Dimension::kDynamic>> X1(make_k_shape(N)),
      Y1(make_k_shape(N));
  std::vector<double> X2(N), Y2(N);
  auto rng = touchstone::random_number_generator();
  std::uniform_real_distribution<double> dist(-1, 1);
  for(auto&& x : all_range(X1))
    x = dist(rng);
  for(int i=0; i<N; ++i)
    X2[i] = dist(rng);
  BENCHMARK("b1") {
    executer(
      execution_mode::nontemporal,
      Y1 = X1*X1
    );
  }
  BENCHMARK("b2") {
    auto expr = make_map_expression(
      numeric_array_expression_tag(),
      [](double x) { return x*x; },
      make_expression(numeric_array_expression_tag(), X1)
      );
    executer(
      execution_mode::force_inline,
      Y1 = expr
    );
  }
  BENCHMARK("b3") {
    for(int i=0; i<N; ++i)
      Y2[i] = X2[i]*X2[i];
  }
  std::cout << "y1[0] = " << Y1(0) << "\tanswer = " << (X1(0)*X1(0)) << "\n";
  std::cout << "y1[N-1] = " << Y1(N-1) << "\tanswer = " << (X1(N-1)*X1(N-1)) << "\n";
  std::cout << "y2[0] = " << Y2[0] << "\n";
  std::cout << "y2[N-1] = " << Y2[N-1] << "\n";
}

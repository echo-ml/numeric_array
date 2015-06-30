#pragma once

#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// ScalarEvaluator
//------------------------------------------------------------------------------
template <class Scalar>
class ScalarEvaluator {
  CONCEPT_ASSERT(execution_context::concept::scalar<Scalar>());

 public:
  ScalarEvaluator(Scalar scalar) : _scalar(scalar) {}

  Scalar operator()(index_t index) const { return _scalar; }

 private:
  Scalar _scalar;
};

template <class Scalar,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>())>
auto make_scalar_evaluator(const Scalar& scalar) {
  return ScalarEvaluator<Scalar>(scalar);
}
}
}

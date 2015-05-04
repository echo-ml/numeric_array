#pragma once

#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

///////////////////////////
// NumericArrayEvaluator //
///////////////////////////

template <class Pointer>
class NumericArrayEvaluator {
  CONCEPT_ASSERT(echo::concept::contiguous_iterator<Pointer>() &&
                 execution_context::concept::scalar<
                     iterator_traits::value_type<Pointer>>());

 public:
  NumericArrayEvaluator(Pointer data) : _data(data) {}
  decltype(auto) operator()(index_t index) const { return *(_data + index); }

 private:
  Pointer _data;
};

template <class Pointer,
          CONCEPT_REQUIRES(echo::concept::contiguous_iterator<Pointer>() &&
                           execution_context::concept::scalar<
                               iterator_traits::value_type<Pointer>>())>
auto make_numeric_array_evaluator(Pointer data) {
  return NumericArrayEvaluator<Pointer>(data);
}
}
}

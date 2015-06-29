#pragma once

#include <echo/execution_context.h>
#include <echo/numeric_array/flatten_evaluator.h>

namespace echo {
namespace numeric_array {

////////////////////
// fuse_evaluator //
////////////////////

template <
    int K, class... Evaluators, class Evaluator,
    CONCEPT_REQUIRES(
        and_c<execution_context::concept::flat_evaluator<Evaluators>()...>() &&
        execution_context::concept::flat_evaluator<Evaluator>())>
auto fuse_evaluator(const Evaluator& evaluator) {
  return evaluator;
}

template <
    int K, class... Evaluators, class Evaluator,
    CONCEPT_REQUIRES(
        and_c<execution_context::concept::k_compatible_evaluator<
            K, Evaluators>()...>() &&
        !and_c<execution_context::concept::flat_evaluator<Evaluators>()...>() &&
        execution_context::concept::k_compatible_evaluator<K, Evaluator>())>
auto fuse_evaluator(const Evaluator& evaluator) {
  return make_k_shaped_evaluator<K>(evaluator);
}
}
}

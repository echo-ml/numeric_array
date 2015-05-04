#pragma once

#include <echo/numeric_array/concept.h>

namespace echo {
namespace numeric_array {

template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        execution_context::concept::structure<Structure1>() &&
        execution_context::concept::structure<Structure2>() &&
        std::is_same<Structure1, Structure2>::value &&
        execution_context::concept::k_compatible_evaluator<K, Evaluator>())>
auto make_conversion_evaluator(const Evaluator& evaluator) {
  return evaluator;
}

template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        execution_context::concept::structure<Structure1>() &&
        execution_context::concept::structure<Structure2>() &&
        std::is_same<Structure1, structure::scalar>::value &&
        execution_context::concept::k_compatible_evaluator<K, Evaluator>())>
auto make_conversion_evaluator(const Evaluator& evaluator) {
  return evaluator;
}
}
}

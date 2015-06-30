#pragma once

#include <echo/numeric_array/concept.h>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// make_conversion_evaluator
//------------------------------------------------------------------------------
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        execution_context::concept::structure<Structure1>() &&
        execution_context::concept::structure<Structure2>() &&
        (std::is_same<Structure1, Structure2>::value ||
         std::is_base_of<Structure2, Structure1>::value) &&
        execution_context::concept::k_compatible_evaluator<K, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  return evaluator;
}

template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        execution_context::concept::structure<Structure1>() &&
        execution_context::concept::structure<Structure2>() &&
        std::is_same<Structure1, structure::scalar>::value &&
        execution_context::concept::k_compatible_evaluator<K, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  return evaluator;
}
}
}

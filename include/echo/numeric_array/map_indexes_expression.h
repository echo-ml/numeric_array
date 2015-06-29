#pragma once

#include <echo/numeric_array/expression.h>
#include <echo/numeric_array/map_indexes_evaluator.h>

namespace echo {
namespace numeric_array {

/////////////////
// map_indexes //
/////////////////

template <
    class Functor, class... Extents,
    CONCEPT_REQUIRES(and_c<k_array::concept::extent<Extents>()...>()),
    CONCEPT_REQUIRES(concept::index_functor<sizeof...(Extents), Functor>())>
auto map_indexes(const Functor& functor, Extents... extents) {
  return make_numeric_array_expression<execution_context::structure::general>(
      make_dimensionality(extents...),
      make_map_indexes_evaluator<sizeof...(Extents)>(functor));
}

template <
    class Structure, class Functor, class... Extents,
    CONCEPT_REQUIRES(and_c<k_array::concept::extent<Extents>()...>()),
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     concept::index_functor<sizeof...(Extents), Functor>())>
auto map_indexes(const Functor& functor, Extents... extents) {
  return make_numeric_array_expression<Structure>(
      make_dimensionality(extents...),
      make_map_indexes_evaluator<sizeof...(Extents)>(functor));
}

template <class Functor, class Dimensionality,
          CONCEPT_REQUIRES(
              concept::index_functor<
                  dimensionality_traits::num_dimensions<Dimensionality>(),
                  Functor>() &&
              k_array::concept::dimensionality<Dimensionality>())>
auto map_indexes(const Functor& functor, const Dimensionality& dimensionality) {
  return make_numeric_array_expression<execution_context::structure::general>(
      dimensionality,
      make_map_indexes_evaluator<
          dimensionality_traits::num_dimensions<Dimensionality>()>(functor));
}

template <class Structure, class Functor, class Dimensionality,
          CONCEPT_REQUIRES(
              execution_context::concept::structure<Structure>() &&
              concept::index_functor<
                  dimensionality_traits::num_dimensions<Dimensionality>(),
                  Functor>() &&
              k_array::concept::dimensionality<Dimensionality>())>
auto map_indexes(const Functor& functor, const Dimensionality& dimensionality) {
  return make_numeric_array_expression<Structure>(
      dimensionality,
      make_map_indexes_evaluator<
          dimensionality_traits::num_dimensions<Dimensionality>()>(functor));
}
}
}

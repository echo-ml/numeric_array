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
    CONCEPT_REQUIRES(
        const_algorithm::and_c<k_array::concept::extent<Extents>()...>()),
    CONCEPT_REQUIRES(concept::index_functor<sizeof...(Extents), Functor>())>
auto map_indexes(const Functor& functor, Extents... extents) {
  return make_numeric_array_expression<execution_context::structure::general>(
      make_k_shape(extents...),
      make_map_indexes_evaluator<sizeof...(Extents)>(functor));
}

template <
    class Structure, class Functor, class... Extents,
    CONCEPT_REQUIRES(
        const_algorithm::and_c<k_array::concept::extent<Extents>()...>()),
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     concept::index_functor<sizeof...(Extents), Functor>())>
auto map_indexes(const Functor& functor, Extents... extents) {
  return make_numeric_array_expression<Structure>(
      make_k_shape(extents...),
      make_map_indexes_evaluator<sizeof...(Extents)>(functor));
}

template <class Functor, class Shape,
          CONCEPT_REQUIRES(
              concept::index_functor<shape_traits::num_dimensions<Shape>(),
                                     Functor>() &&
              k_array::concept::shape<Shape>())>
auto map_indexes(const Functor& functor, const Shape& shape) {
  return make_numeric_array_expression<execution_context::structure::general>(
      shape, make_map_indexes_evaluator<shape_traits::num_dimensions<Shape>()>(
                 functor));
}

template <class Structure, class Functor, class Shape,
          CONCEPT_REQUIRES(
              execution_context::concept::structure<Structure>() &&
              concept::index_functor<shape_traits::num_dimensions<Shape>(),
                                     Functor>() &&
              k_array::concept::shape<Shape>())>
auto map_indexes(const Functor& functor, const Shape& shape) {
  return make_numeric_array_expression<Structure>(
      shape, make_map_indexes_evaluator<shape_traits::num_dimensions<Shape>()>(
                 functor));
}
}
}

#pragma once

namespace echo { namespace numeric_array {

//------------------------------------------------------------------------------
// get_single_index_stride
//------------------------------------------------------------------------------
template <class Shape,
          CONCEPT_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1)>
auto get_single_index_stride(const Shape& shape) {
  return get_stride<shape_traits::free_dimension<Shape>()>(shape);
}

template <class Shape,
          CONCEPT_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 0)>
auto get_single_index_stride(const Shape& shape) {
  return 1_index;
}

//------------------------------------------------------------------------------
// get_single_index_extent
//------------------------------------------------------------------------------
template<class Shape,
          CONCEPT_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1)>
auto get_single_index_extent(const Shape& shape) {
  return get_extent<shape_traits::free_dimension<Shape>()>(shape);
}

template <class Shape,
          CONCEPT_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 0)>
auto get_single_index_extent(const Shape& shape) {
  return 1_index;
}

}}

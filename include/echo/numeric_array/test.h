#pragma once

#include <echo/numeric_array/concept.h>
#include <echo/test.h>

namespace echo {
namespace numeric_array {

namespace detail {
namespace test {

template <int I, class Values, class Shape, class Functor,
          CONCEPT_REQUIRES(I == shape_traits::num_dimensions<Shape>())>
void check_impl(Values values, const Shape& shape, const Functor& functor) {
}

template <int I, class Values, class Shape, class Functor,
          CONCEPT_REQUIRES(I != shape_traits::num_dimensions<Shape>())>
void check_impl(Values values, const Shape& shape, const Functor& functor) {
  CHECK(get_extent<I>(shape) == values.size());
  index_t index=0;
  for(auto values_i : values) {
    auto functor_new = [&](auto value, auto... indexes) {
      functor(value, index, indexes...);
    };
    ++index;
  }
}

template <int I, class Array1, CONCEPT_REQUIRES(I != 0)>
void check_impl(const Array1& array1,
                Initializer<k_array_traits::value_type<Array1>,
                            k_array_traits::num_dimensions<Array1>()> values) 
{
  const auto& shape = array1.shape();
  CHECK(get_extent<I>(shape) == values.size());
  index_t index=0;
  for(auto values_i : values) {
       
  }
}

template <class Array1, class Array2,
          CONCEPT_REQUIRES(concept::numeric_array<Array1>())>
void check(const Array1& array1,
           Initializer<k_array_traits::value_type<Array1>,
           k_array_traits::num_dimensions<Array1>()> values) {
  const auto& shape = array1.shape(); 
}
}
}
}
}

//#define ARRAY_EQUAL(ARRAY1, ARRAY2

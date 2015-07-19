#pragma once

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <echo/numeric_array/numeric_array_traits.h>
#include <echo/k_array.h>

namespace echo {
namespace numeric_array {
template <class NumericArray, class... Slices,
          CONCEPT_REQUIRES(concept::numeric_subarray<uncvref_t<NumericArray>,
                                                        Slices...>())>
auto make_numeric_subarray(NumericArray&& numeric_array,
                           const Slices&... slices) {
  auto data = &numeric_array(k_array::slice::get_first_index(slices)...);
  auto shape = make_subshape(numeric_array.shape(), slices...);
  return make_numeric_array_view<
      numeric_array_traits::structure<uncvref_t<NumericArray>>,
      numeric_array_traits::memory_backend_tag<uncvref_t<NumericArray>>>(data,
                                                                         shape);
}
}
}

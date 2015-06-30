#pragma once

#include <echo/numeric_array/concept.h>
#include <echo/utility/range.h>
#include <echo/k_array.h>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// all_begin
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_begin(NumericArray&& numeric_array) {
  return numeric_array.data();
}

//------------------------------------------------------------------------------
// all_end
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_end(NumericArray&& numeric_array) {
  return all_begin(numeric_array) + get_num_elements(numeric_array);
}

//------------------------------------------------------------------------------
// all_cbegin
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_cbegin(NumericArray&& numeric_array) {
  return numeric_array.const_data();
}

//------------------------------------------------------------------------------
// all_cend
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_cend(NumericArray&& numeric_array) {
  return all_cbegin(numeric_array) + get_num_elements(numeric_array);
}

//------------------------------------------------------------------------------
// all_range
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_range(NumericArray&& numeric_array) {
  return make_range(all_begin(numeric_array), all_end(numeric_array));
}

//------------------------------------------------------------------------------
// all_crange
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>())>
auto all_crange(NumericArray&& numeric_array) {
  return make_range(all_cbegin(numeric_array), all_cend(numeric_array));
}
}
}
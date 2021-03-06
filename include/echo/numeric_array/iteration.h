#pragma once

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/utility.h>
#include <echo/k_array.h>
#include <echo/iterator.h>

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
  return all_begin(numeric_array.k_array());
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
  return all_end(numeric_array.k_array());
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
  return all_cbegin(numeric_array.k_array());
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
  return all_cend(numeric_array.k_array());
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
  return all_range(numeric_array.k_array());
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
  return all_crange(numeric_array.k_array());
}

//------------------------------------------------------------------------------
// begin
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>() &&
      concept::iterable_numeric_array<uncvref_t<NumericArray>>())>
auto begin(NumericArray&& numeric_array) {
  return make_strided_iterator(numeric_array.data(),
                               get_single_index_stride(numeric_array.shape()));
}

//------------------------------------------------------------------------------
// end
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>() &&
      concept::iterable_numeric_array<uncvref_t<NumericArray>>())>
auto end(NumericArray&& numeric_array) {
  return make_strided_iterator(numeric_array.data(),
                               get_single_index_stride(numeric_array.shape())) +
         get_single_index_extent(numeric_array.shape());
}

//------------------------------------------------------------------------------
// cbegin
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>() &&
      concept::iterable_numeric_array<uncvref_t<NumericArray>>())>
auto cbegin(NumericArray&& numeric_array) {
  return make_strided_iterator(numeric_array.const_data(),
                               get_single_index_stride(numeric_array.shape()));
}

//------------------------------------------------------------------------------
// cend
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::numeric_array<execution_context::structure::general,
                                     uncvref_t<NumericArray>>() &&
      concept::iterable_numeric_array<uncvref_t<NumericArray>>())>
auto cend(NumericArray&& numeric_array) {
  return make_strided_iterator(numeric_array.const_data(),
                               get_single_index_stride(numeric_array.shape())) +
         get_single_index_extent(numeric_array.shape());
}

//------------------------------------------------------------------------------
// begin
//------------------------------------------------------------------------------
template <class Expression,
          CONCEPT_REQUIRES(concept::dimensioned_expression<1, Expression>())>
auto begin(const Expression& expression) {
  return make_index_functor_iterator(0, expression.evaluator());
}

//------------------------------------------------------------------------------
// end
//------------------------------------------------------------------------------
template <class Expression,
          CONCEPT_REQUIRES(concept::dimensioned_expression<1, Expression>())>
auto end(const Expression& expression) {
  return make_index_functor_iterator(get_num_elements(expression),
                                     expression.evaluator());
}
}
}

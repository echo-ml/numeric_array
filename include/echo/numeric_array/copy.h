#pragma once

#define DETAIL_NS detail_copy

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/numeric_array_traits.h>
#include <echo/numeric_array/map_indexes_expression.h>
#include <echo/memory.h>
#include <echo/execution_context.h>
#include <echo/contract.h>

namespace echo {
namespace numeric_array {

namespace DETAIL_NS {
//------------------------------------------------------------------------------
// copy
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class SourceNumericArray,
    class DestinationNumericArray,
    CONCEPT_REQUIRES(
        execution_context::concept::expression_executer<ExecutionContext>() &&
        concept::compatible_numeric_arrays<
            SourceNumericArray, uncvref_t<DestinationNumericArray>>() &&
        concept::modifiable_numeric_array_forward<DestinationNumericArray>() &&
        concept::contiguous_numeric_array<SourceNumericArray>() &&
        concept::contiguous_numeric_array<
            uncvref_t<DestinationNumericArray>>() &&
        concept::numeric_array<execution_context::structure::general,
                               SourceNumericArray>() &&
        concept::numeric_array<execution_context::structure::general,
                               DestinationNumericArray>())>
void copy_impl(overload::choice<0>, const ExecutionContext& execution_context,
               const SourceNumericArray& source,
               DestinationNumericArray&& destination) {
  copy(source.k_array(), destination.k_array());
}

template <
    class ExecutionContext, class SourceNumericArray,
    class DestinationNumericArray,
    CONCEPT_REQUIRES(
        execution_context::concept::expression_executer<ExecutionContext>() &&
        concept::numeric_array_convertible_to<
            SourceNumericArray, uncvref_t<DestinationNumericArray>>() &&
        concept::modifiable_numeric_array_forward<DestinationNumericArray>())>
void copy_impl(overload::choice<1>, const ExecutionContext& execution_context,
               const SourceNumericArray& source,
               DestinationNumericArray&& destination) {
  using DestinationScalar = uncvref_t<decltype(*destination.data())>;
  auto assignment_functor = [&](auto... indexes) {
    return destination(access_mode::raw, indexes...) =
               static_cast<DestinationScalar>(
                   source(access_mode::raw, indexes...));
  };
  auto expression =
      map_indexes(assignment_functor, get_dimensionality(destination));
  execution_context(execution_mode::nontemporal | execution_mode::simd |
                        execution_mode::parallel_coarse,
                    expression);
}
}

//------------------------------------------------------------------------------
// copy
//------------------------------------------------------------------------------
template <class ExecutionContext, class SourceNumericArray,
          class DestinationNumericArray>
auto copy(const ExecutionContext& execution_context,
          const SourceNumericArray& source,
          DestinationNumericArray&& destination)
    -> decltype(DETAIL_NS::copy_impl(
        overload::selector(), execution_context, source,
        std::forward<DestinationNumericArray>(destination))) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(get_dimensionality(source) ==
                    get_dimensionality(destination));
  };
  DETAIL_NS::copy_impl(overload::selector(), execution_context, source,
                       std::forward<DestinationNumericArray>(destination));
}
}
}

#undef DETAIL_NS

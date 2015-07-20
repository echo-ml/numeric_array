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
        concept::contiguous_numeric_array<SourceNumericArray>() &&
        concept::contiguous_numeric_array<
            uncvref_t<DestinationNumericArray>>() &&
        concept::numeric_array<execution_context::structure::general,
                               SourceNumericArray>() &&
        concept::numeric_array<execution_context::structure::general,
                               DestinationNumericArray>())>
void copy(const ExecutionContext& execution_context,
          const SourceNumericArray& source,
          DestinationNumericArray&& destination) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(get_dimensionality(source) ==
                    get_dimensionality(destination));
  };
  copy(source.k_array(), destination.k_array());
}

template <
    class ExecutionContext, class SourceNumericArray,
    class DestinationNumericArray,
    CONCEPT_REQUIRES(
        execution_context::concept::expression_executer<ExecutionContext>() &&
        concept::compatible_numeric_arrays<
            SourceNumericArray, uncvref_t<DestinationNumericArray>>() &&
        !(concept::contiguous_numeric_array<SourceNumericArray>() &&
          concept::contiguous_numeric_array<
              uncvref_t<DestinationNumericArray>>() &&
          concept::numeric_array<execution_context::structure::general,
                                 SourceNumericArray>() &&
          concept::numeric_array<execution_context::structure::general,
                                 DestinationNumericArray>()))>
void copy(const ExecutionContext& execution_context,
          const SourceNumericArray& source,
          DestinationNumericArray&& destination) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(get_dimensionality(source) ==
                    get_dimensionality(destination));
  };
  auto assignment_functor = [&](auto... indexes) {
    return destination(access_mode::raw, indexes...) =
               source(access_mode::raw, indexes...);
  };
  auto expression =
      map_indexes(assignment_functor, get_dimensionality(destination));
  execution_context(execution_mode::nontemporal | execution_mode::simd |
                        execution_mode::parallel_coarse,
                    expression);
}
}
}

#undef DETAIL_NS

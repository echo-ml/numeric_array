#pragma once

#define DETAIL_NS detail_copy

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/numeric_array_traits.h>
#include <echo/numeric_array/map_indexes_expression.h>
#include <echo/memory.h>
#include <echo/execution_context.h>

// namespace echo {
// namespace numeric_array {

//------------------------------------------------------------------------------
// copyable
//------------------------------------------------------------------------------

// namespace DETAIL_NS {
// namespace concept {
// template <class A, class B>
// constexpr bool copyable() {
//   return std::is_same<k_array_traits::value_type<A>,
//                       k_array_traits::value_type<uncvref_t<B>>>::value &&
//          std::is_same<k_array_traits::shape_type<A>,
//                       k_array_traits::shape_type<uncvref_t<B>>>::value &&
//          std::is_same<numeric_array_traits::structure<A>,
//                       numeric_array_traits::structure<uncvref_t<B>>>::value &&
//          echo::numeric_array::concept::modifiable_numeric_array_forward<B>();
// }
// }
// }
//

//------------------------------------------------------------------------------
// copy
//------------------------------------------------------------------------------

// template <
//     class ExecutionContext, class NumericArray1, class NumericArray2,
//     CONCEPT_REQUIRES(
//         execution_context::concept::expression_executer<ExecutionContext>() &&
//         DETAIL_NS::concept::copyable<NumericArray1, NumericArray2>() &&
//         concept::contiguous_numeric_array<NumericArray1>() &&
//         concept::contiguous_numeric_array<uncvref_t<NumericArray2>>())>
// void copy(const ExecutionContext& execution_context,
//           const NumericArray1& source, NumericArray2&& destination) {
//   assert(get_num_elements(source) == get_num_elements(destination));
//   std::memcpy(destination.data(), source.data(),
//               get_num_elements(source) *
//                   sizeof(k_array_traits::value_type<NumericArray1>));
// }
//
// template <
//     class ExecutionContext, class NumericArray1, class NumericArray2,
//     CONCEPT_REQUIRES(
//         execution_context::concept::expression_executer<ExecutionContext>() &&
//         DETAIL_NS::concept::copyable<NumericArray1, NumericArray2>() &&
//         (!concept::contiguous_numeric_array<NumericArray1>() ||
//          !concept::contiguous_numeric_array<uncvref_t<NumericArray2>>()))>
// void copy(const ExecutionContext& execution_context,
//           const NumericArray1& source, NumericArray2&& destination) {
//   assert(get_num_elements(source) == get_num_elements(destination));
//   auto assignment_functor = [&](auto... indexes) {
//     return destination(access_mode::raw, indexes...) =
//                source(access_mode::raw, indexes...);
//   };
//   auto expression = map_indexes(assignment_functor, destination.shape());
//   execution_context(execution_mode::simd | execution_mode::parallel_coarse,
//                     expression);
// }
// }
// }

#undef DETAIL_NS

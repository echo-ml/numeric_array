#pragma once

#define DETAIL_NS detail_map_expression

#include <echo/numeric_array/expression.h>
#include <echo/numeric_array/map_evaluator.h>
#include <echo/numeric_array/conversion_evaluator.h>

namespace echo {
namespace numeric_array {

//////////////////////////////////////
// get_first_dimensioned_expression //
//////////////////////////////////////

namespace DETAIL_NS {
template <class ExpressionFirst, class... ExpressionsRest,
          CONCEPT_REQUIRES(concept::dimensioned_expression<ExpressionFirst>())>
const auto& get_first_dimensioned_expression(
    const ExpressionFirst& expression_first,
    const ExpressionsRest&... expressions_rest) {
  return expression_first;
}

template <class ExpressionFirst, class... ExpressionsRest,
          CONCEPT_REQUIRES(concept::scalar_expression<ExpressionFirst>())>
const auto& get_first_dimensioned_expression(
    const ExpressionFirst& expression_first,
    const ExpressionsRest&... expressions_rest) {
  return get_first_dimensioned_expression(expressions_rest...);
}
}

/////////////////////////
// make_map_expression //
/////////////////////////

namespace DETAIL_NS {
// bug with intel compiler requires breaking this out separately
template <class Functor, class... Expressions>
constexpr bool check_arguments() {
  return concept::compatible_expressions<Expressions...>() &&
         echo::concept::callable<
             Functor,
             type_traits::functor_return_type<
                 expression_traits::evaluator_type<Expressions>>...>() &&
         concept::compatible_functor_evaluators<
             Functor, expression_traits::evaluator_type<Expressions>...>();
}
}

template <
    class Functor, class... Expressions,
    CONCEPT_REQUIRES(DETAIL_NS::check_arguments<Functor, Expressions...>())>
auto make_map_expression(numeric_array_expression_tag, const Functor& functor,
                         const Expressions&... expressions) {
  using structure =
      structure_traits::fuse<expression_traits::structure<Expressions>...>;
  const auto& dimensionality =
      DETAIL_NS::get_first_dimensioned_expression(expressions...)
          .dimensionality();
  constexpr int K = dimensionality_traits::num_dimensions<
      uncvref_t<decltype(dimensionality)>>();
  return make_numeric_array_expression<structure>(
      dimensionality,
      make_map_evaluator<K>(
          functor, make_conversion_evaluator<K>(
                       expression_traits::structure<Expressions>(), structure(),
                       expressions.evaluator())...));
}

///////////////////////////////////////
// make_binary_arithmetic_expression //
///////////////////////////////////////

template <class Functor, class Lhs, class Rhs,
          CONCEPT_REQUIRES(DETAIL_NS::check_arguments<Functor, Lhs, Rhs>())>
auto make_binary_arithmetic_expression(numeric_array_expression_tag,
                                       const Functor& functor, const Lhs& lhs,
                                       const Rhs& rhs) {
  return make_map_expression(numeric_array_expression_tag(), functor, lhs, rhs);
}

template <class Functor, class Lhs, class Rhs,
          CONCEPT_REQUIRES(DETAIL_NS::check_arguments<Functor, Lhs, Rhs>())>
auto make_assignment_expression(numeric_array_expression_tag,
                                const Functor& functor, const Lhs& lhs,
                                const Rhs& rhs) {
  return make_map_expression(numeric_array_expression_tag(), functor, lhs, rhs);
}
}
}

#undef DETAIL_NS

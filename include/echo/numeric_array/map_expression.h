#pragma once

#include <echo/numeric_array/expression.h>
#include <echo/numeric_array/map_evaluator.h>
#include <echo/numeric_array/conversion_evaluator.h>

namespace echo {
namespace numeric_array {

/////////////////////////////////
// get_first_shaped_expression //
/////////////////////////////////

namespace detail {
namespace map_expression {

template <class ExpressionFirst, class... ExpressionsRest,
          CONCEPT_REQUIRES(concept::shaped_expression<ExpressionFirst>())>
const auto& get_first_shaped_expression(
    const ExpressionFirst& expression_first,
    const ExpressionsRest&... expressions_rest) {
  return expression_first;
}

template <class ExpressionFirst, class... ExpressionsRest,
          CONCEPT_REQUIRES(concept::scalar_expression<ExpressionFirst>())>
const auto& get_first_shaped_expression(
    const ExpressionFirst& expression_first,
    const ExpressionsRest&... expressions_rest) {
  return get_first_shaped_expression(expressions_rest...);
}
}
}

/////////////////////////
// make_map_expression //
/////////////////////////

namespace detail {
namespace map_expression {
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
}

template <class Functor, class... Expressions,
          CONCEPT_REQUIRES(detail::map_expression::check_arguments<
              Functor, Expressions...>())>
auto make_map_expression(numeric_array_expression_tag, const Functor& functor,
                         const Expressions&... expressions) {
  using structure =
      structure_traits::fuse_t<expression_traits::structure<Expressions>...>;
  const auto& shape =
      detail::map_expression::get_first_shaped_expression(expressions...)
          .shape();
  constexpr int K = shape_traits::num_dimensions<decltype(shape)>();
  return make_numeric_array_expression<structure>(
      shape,
      make_map_evaluator<K>(
          functor, make_conversion_evaluator<
                       K, expression_traits::structure<Expressions>, structure>(
                       expressions.evaluator())...));
}

///////////////////////////////////////
// make_binary_arithmetic_expression //
///////////////////////////////////////

template <class Functor, class Lhs, class Rhs,
          CONCEPT_REQUIRES(
              detail::map_expression::check_arguments<Functor, Lhs, Rhs>())>
auto make_binary_arithmetic_expression(numeric_array_expression_tag,
                                       const Functor& functor, const Lhs& lhs,
                                       const Rhs& rhs) {
  return make_map_expression(numeric_array_expression_tag(), functor, lhs, rhs);
}

template <class Functor, class Lhs, class Rhs,
          CONCEPT_REQUIRES(
              detail::map_expression::check_arguments<Functor, Lhs, Rhs>())>
auto make_assignment_expression(numeric_array_expression_tag,
                                const Functor& functor, const Lhs& lhs,
                                const Rhs& rhs) {
  return make_map_expression(numeric_array_expression_tag(), functor, lhs, rhs);
}

/////////
// map //
/////////

// template<class Functor, class... Expressions,
//   CONCEPT_REQUIRES(
//       detail::map_expression::check_arguments<Functor, Expressions...>())
// auto map(const Functor& functor, Expressions&&... expressions) {
//   return make_map_expression(
//     
// }

}
}

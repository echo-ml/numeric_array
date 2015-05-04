#pragma once

#include <echo/assert.h>
#include <echo/numeric_array/null_shape.h>
#include <echo/numeric_array/evaluator.h>
#include <echo/numeric_array/scalar_evaluator.h>
#include <echo/numeric_array/numeric_array_evaluator.h>
#include <echo/numeric_array/conversion_evaluator.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/concept.h>
#include <echo/k_array.h>
#include <echo/execution_context.h>
#include <echo/numeric_array/structure_traits.h>

namespace echo {
namespace numeric_array {

////////////////////////////
// NumericArrayExpression //
////////////////////////////

template <class Shape, class Structure, class Evaluator>
class NumericArrayExpression : Shape {
 public:
  NumericArrayExpression(const Shape& shape, const Evaluator& evaluator)
      : Shape(shape), _evaluator(evaluator) {}
  using expression_template_tag = numeric_array_expression_tag;
  using structure = Structure;

  const Evaluator evaluator() const { return _evaluator; }
  const Shape& shape() const { return static_cast<const Shape&>(*this); }

 private:
  Evaluator _evaluator;
};

template <
    class Structure, class Shape, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     k_array::concept::shape<Shape>() &&
                     execution_context::concept::k_compatible_evaluator<
                         shape_traits::num_dimensions<Shape>(), Evaluator>())>
auto make_numeric_array_expression(const Shape& shape,
                                   const Evaluator& evaluator) {
  return NumericArrayExpression<Shape, Structure, Evaluator>(shape, evaluator);
}

template <
    class Structure, class Shape, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     std::is_same<Shape, NullShape>::value &&
                     execution_context::concept::flat_evaluator<Evaluator>())>
auto make_numeric_array_expression(const Shape& shape,
                                   const Evaluator& evaluator) {
  return NumericArrayExpression<Shape, Structure, Evaluator>(shape, evaluator);
}

/////////////////////
// make_expression //
/////////////////////

template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::numeric_array<uncvref_t<NumericArray>>())>
auto make_expression(numeric_array_expression_tag,
                     NumericArray&& numeric_array) {
  return make_numeric_array_expression<
      expression_traits::structure<NumericArray>>(
      numeric_array.shape(),
      make_numeric_array_evaluator(numeric_array.data()));
}

template <class Scalar,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>())>
auto make_expression(numeric_array_expression_tag, const Scalar& scalar) {
  return make_numeric_array_expression<structure::scalar>(
      NullShape(), make_scalar_evaluator(scalar));
}

template <class Shape, class Structure, class Evaluator>
auto make_expression(
    numeric_array_expression_tag,
    const NumericArrayExpression<Shape, Structure, Evaluator>& expression) {
  return expression;
}

/////////////////////////////////////
// make_binary_function_expression //
/////////////////////////////////////

// template<
//     class Shape
//   , class Functor
//   , class Lhs
//   , class Rhs
// >
// auto make_binary_function_expression(
//         const Shape& shape
//       , const Functor& functor
//       , const Lhs& lhs
//       , const Rhs& rhs)
// {
//   return make_numeric_array_expression(
//               shape
//             , make_binary_function_evaluator(
//                       functor
//                     , lhs.evaluator()
//                     , rhs.evaluator())
//   );
// }

///////////////////////////////////////
// make_binary_arithmetic_expression //
///////////////////////////////////////

// template <class Functor, class Lhs, class Rhs>
// auto make_binary_arithmetic_expression(numeric_array_expression_tag,
//                                        const Functor& functor, const Lhs& lhs,
//                                        const Rhs& rhs) {
  // assert_any_shaped_match(lhs, rhs);
  // return make_binary_function_expression(get_first_shaped(lhs, rhs), functor,
  // lhs, rhs);
// }

////////////////////////////////
// make_assignment_expression //
////////////////////////////////

// template<
//     class Functor
//   , class Shape1
//   , class Evaluator1
//   , class Rhs
// >
// auto make_assignment_expression(
//         numeric_array_expression_tag
//       , const Functor& functor
//       , const NumericArrayExpression<Shape1, Evaluator1>& lhs
//       , const Rhs& rhs)
// {
//   assert_any_shaped_match(lhs, rhs);
//   return make_binary_function_expression(get_first_shaped(lhs, rhs), functor,
//   lhs, rhs);
// }

/////////////////////////
// make_map_expression //
/////////////////////////

// template<
//     class Functor
//   , class... Nodes
// >
// auto make_map_expression(
//         numeric_array_expression_tag
//       , const Functor& functor
//       , const Nodes&... nodes)
// {
// assert_any_shaped_match(nodes...);
// return make_numeric_array_expression(
//     get_first_shaped(nodes...)
//   , make_numeric_array_map_evaluator(functor, nodes.evaluator()...)
// );
// }
}
}  // end namespace echo::numeric_array

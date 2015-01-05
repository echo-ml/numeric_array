#pragma once

#include <echo/assert.h>
#include <echo/numeric_array/evaluator.h>
#include <echo/k_array/shape.h>

namespace echo { namespace numeric_array {

//////////////////////////////////
// numeric_array_expression_tag //
//////////////////////////////////

struct numeric_array_expression_tag {};

////////////////////////////
// NumericArrayExpression //
////////////////////////////

template<class Shape, class Evaluator>
class NumericArrayExpression
  : Shape
{
 public:
  NumericArrayExpression(const Shape& shape, const Evaluator& evaluator)
    : Shape(shape)
    , _evaluator(evaluator)
  {}
  using expression_template_tag = numeric_array_expression_tag;

  const Evaluator evaluator() const { return _evaluator; }
  const Shape& shape() const { return static_cast<const Shape&>(*this); }
 private:
  Evaluator _evaluator;
};

template<class Shape, class Evaluator>
auto make_numeric_array_expression(const Shape& shape, const Evaluator& evaluator) {
  return NumericArrayExpression<Shape, Evaluator>(shape, evaluator);
}

//////////////////////
// ScalarExpression //
//////////////////////

template<class Evaluator>
class ScalarExpression {
 public:
  ScalarExpression(const Evaluator& evaluator)
    : _evaluator(evaluator)
  {}
  const Evaluator& evaluator() const { return _evaluator; }
 private:
  Evaluator _evaluator;
};

template<class Evaluator>
auto make_scalar_expression(const Evaluator& evaluator) {
  return ScalarExpression<Evaluator>(evaluator);
}

/////////////////////
// make_expression //
/////////////////////

template<
    class Array
  , enable_if<is_k_array<Array>> = 0
>
auto make_expression(numeric_array_expression_tag, Array&& array) {
  return make_numeric_array_expression(
      array.shape()
    , make_numeric_array_evaluator(array.data())
  );
}

template<
    class Scalar
  , disable_if<is_k_array<Scalar>> = 0
>
auto make_expression(numeric_array_expression_tag, const Scalar& scalar) {
  return make_scalar_expression(make_scalar_evaluator(scalar));
}

template<
    class Shape
  , class Evaluator
>
auto make_expression(numeric_array_expression_tag
                   , const NumericArrayExpression<Shape, Evaluator>& expression)
{
  return expression;
}

/////////////////////////////////////
// make_binary_function_expression //
/////////////////////////////////////

template<
    class Shape
  , class Functor
  , class Lhs
  , class Rhs
>
auto make_binary_function_expression(
        const Shape& shape
      , const Functor& functor
      , const Lhs& lhs
      , const Rhs& rhs)
{
  return make_numeric_array_expression(
              shape
            , make_binary_function_evaluator(
                      functor
                    , lhs.evaluator()
                    , rhs.evaluator())
  );
}

///////////////////////////////////////
// make_binary_arithmetic_expression //
///////////////////////////////////////

template<
    class Functor
  , class Lhs
  , class Rhs
>
auto make_binary_arithmetic_expression(
        numeric_array_expression_tag
      , const Functor& functor
      , const Lhs& lhs
      , const Rhs& rhs)
{
  assert_any_shaped_match(lhs, rhs);
  return make_binary_function_expression(get_first_shaped(lhs, rhs), functor, lhs, rhs);
}

////////////////////////////////
// make_assignment_expression //
////////////////////////////////

template<
    class Functor
  , class Shape1
  , class Evaluator1
  , class Rhs
>
auto make_assignment_expression(
        numeric_array_expression_tag
      , const Functor& functor
      , const NumericArrayExpression<Shape1, Evaluator1>& lhs
      , const Rhs& rhs)
{
  assert_any_shaped_match(lhs, rhs);
  return make_binary_function_expression(get_first_shaped(lhs, rhs), functor, lhs, rhs);
}

/////////////////////////
// make_map_expression //
/////////////////////////


template<
    class Functor
  , class... Nodes
>
auto make_map_expression(
        numeric_array_expression_tag
      , const Functor& functor
      , const Nodes&... nodes)
{
  assert_any_shaped_match(nodes...);
  return make_numeric_array_expression(
      get_first_shaped(nodes...)
    , make_numeric_array_map_evaluator(functor, nodes.evaluator()...)
  );
}

}} //end namespace echo::numeric_array

#pragma once

#include <echo/numeric_array/evaluator.h>

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
  : public Shape
{
 public:
  NumericArrayExpression(const Shape& shape, const Evaluator& evaluator)
    : Shape(shape)
    , _evaluator(evaluator)
  {}
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
auto make_expression(numeric_array_expression_tag, const Array& array) {
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

///////////////////////////////////////
// make_binary_arithmetic_expression //
///////////////////////////////////////

template<
    class Functor
  , class Shape1
  , class Shape2
  , class Evaluator1
  , class Evaluator2
>
auto make_binary_arithmetic_expression(
        numeric_array_expression_tag
      , const Functor& functor
      , const NumericArrayExpression<Shape1, Evaluator1>& lhs
      , const NumericArrayExpression<Shape2, Evaluator2>& rhs)
{
  return make_numeric_array_expression(
              lhs.shape()
            , functor
            , make_binary_function_evaluator(
                      functor
                    , lhs.evaluator()
                    , rhs.evaluator())
  );
}

}} //end namespace echo::numeric_array

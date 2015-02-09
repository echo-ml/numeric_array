#pragma once

#include <echo/k_array.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression.h>

namespace echo { namespace numeric_array {

//////////////////
// NumericArray //
//////////////////

template<
    class Scalar
  , class Shape
  , class Allocator = std::allocator<Scalar>
>
class NumericArray
  : public KArray<Scalar, Shape, Allocator>
  , public ExpressionTemplateAssignment<
        NumericArray<Scalar, Shape, Allocator>
      , numeric_array_expression_tag
      , Scalar
    >
  , public KArrayAssignment<NumericArray<Scalar, Shape, Allocator>, Scalar>
{
  using BaseKArray = KArray<Scalar, Shape, Allocator>;
  using AssignmentBase = KArrayAssignment<NumericArray<Scalar, Shape, Allocator>, Scalar>;
  using BaseExpressionTemplateAssignment = 
      ExpressionTemplateAssignment<
          NumericArray<Scalar, Shape, Allocator>
        , numeric_array_expression_tag
        , Scalar
      >;
 public:
  using BaseKArray::BaseKArray; 
  using AssignmentBase::operator=;
  using BaseExpressionTemplateAssignment::operator=;
};

}} //end namespace echo::numeric_array

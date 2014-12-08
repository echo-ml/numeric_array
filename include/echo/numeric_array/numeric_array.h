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
        numeric_array_expression_tag
      , NumericArray<Scalar, Shape, Allocator>
    >
{
  using BaseKArray = KArray<Scalar, Shape, Allocator>;
  using BaseExpressionTemplateAssignment = 
      ExpressionTemplateAssignment<
          numeric_array_expression_tag
        , NumericArray<Scalar, Shape, Allocator>
      >;
 public:
  using BaseKArray::BaseKArray; 
  using BaseKArray::operator=;
  using BaseExpressionTemplateAssignment::operator=;
};

}} //end namespace echo::numeric_array

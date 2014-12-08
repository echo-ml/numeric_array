#pragma once

#include <echo/k_array.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression.h>

namespace echo { namespace numeric_array {

//////////////////////
// NumericArrayView //
//////////////////////

template<
    class Pointer
  , class Shape
>
class NumericArrayView
  : public KArrayView<Pointer, Shape>
  , public ExpressionTemplateConstAssignment<
        numeric_array_expression_tag
      , NumericArrayView<Pointer, Shape>
    >
{
  using BaseKArray = KArrayView<Pointer, Shape>;
  using BaseExpressionTemplateAssignment = 
      ExpressionTemplateConstAssignment<
          numeric_array_expression_tag
        , NumericArrayView<Pointer, Shape>
      >;
 public:
  using BaseKArray::BaseKArray; 
  using BaseKArray::operator=;
  using BaseExpressionTemplateAssignment::operator=;
};

}} //end namespace echo::numeric_array

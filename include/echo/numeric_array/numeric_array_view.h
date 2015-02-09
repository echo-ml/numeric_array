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
        NumericArrayView<Pointer, Shape>
      , numeric_array_expression_tag
      , typename std::iterator_traits<Pointer>::value_type
    >
{
  using BaseKArray = KArrayView<Pointer, Shape>;
  // using AssignmentBase = KArrayAssignment<NumericArrayView<Pointer, Shape>, Scalar>;
  using BaseExpressionTemplateAssignment = ExpressionTemplateConstAssignment<
      NumericArrayView<Pointer, Shape>
    , numeric_array_expression_tag
    , typename std::iterator_traits<Pointer>::value_type
  >;
 public:
  using BaseKArray::BaseKArray; 
  using BaseExpressionTemplateAssignment::operator=;
  // using AssignmentBase::operator=;
};

}} //end namespace echo::numeric_array

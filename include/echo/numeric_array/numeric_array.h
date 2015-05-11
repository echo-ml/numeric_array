#pragma once

#include <echo/k_array.h>
#include <echo/simd_allocator.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression_template_tag.h>
#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

//////////////////
// NumericArray //
//////////////////

template <class Scalar, class Shape,
          class Structure = execution_context::structure::general,
          class Allocator = SimdAllocator<Scalar> >
class NumericArray
    : public KArray<Scalar, Shape, Allocator>,
      public expression_template::ExpressionTemplateAssignment<
          NumericArray<Scalar, Shape, Structure, Allocator>,
          numeric_array_expression_tag, Scalar>,
      public KArrayAssignment<NumericArray<Scalar, Shape, Structure, Allocator>,
                              Scalar> {
  using BaseKArray = KArray<Scalar, Shape, Allocator>;
  using AssignmentBase =
      KArrayAssignment<NumericArray<Scalar, Shape, Structure, Allocator>,
                       Scalar>;
  using BaseExpressionTemplateAssignment = expression_template::ExpressionTemplateAssignment<
      NumericArray<Scalar, Shape, Structure, Allocator>,
      numeric_array_expression_tag, Scalar>;

 public:
  using structure = Structure;
  using BaseKArray::BaseKArray;
  using AssignmentBase::operator=;
  using BaseExpressionTemplateAssignment::operator=;
};

}
}  // end namespace echo::numeric_array

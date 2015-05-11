#pragma once

#include <echo/k_array.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression_template_tag.h>
#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

//////////////////////
// NumericArrayView //
//////////////////////

template <class Pointer, class Shape,
          class Structure = execution_context::structure::general>
class NumericArrayView
    : public KArrayView<Pointer, Shape>,
      public expression_template::ExpressionTemplateConstAssignment<
          NumericArrayView<Pointer, Shape, Structure>,
          numeric_array_expression_tag,
          typename std::iterator_traits<Pointer>::value_type> {
  using BaseKArrayView = KArrayView<Pointer, Shape>;

 public:
  using structure = Structure;
  using BaseKArrayView::operator=;
  using BaseKArrayView::BaseKArrayView;
  using expression_template::ExpressionTemplateConstAssignment<
      NumericArrayView, numeric_array_expression_tag,
      typename std::iterator_traits<Pointer>::value_type>::
  operator=;
};

/////////////////////////////
// make_numeric_array_view //
/////////////////////////////

template <class Pointer, class Shape,
          CONCEPT_REQUIRES(echo::concept::contiguous_iterator<Pointer>() &&
                           k_array::concept::shape<Shape>())>
auto make_numeric_array_view(Pointer data, const Shape& shape) {
  return NumericArrayView<Pointer, Shape>(data, shape);
}

template <class Structure, class Pointer, class Shape,
          CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                           echo::concept::contiguous_iterator<Pointer>() &&
                           k_array::concept::shape<Shape>())>
auto make_numeric_array_view(Pointer data, const Shape& shape) {
  return NumericArrayView<Pointer, Shape, Structure>(data, shape);
}

}
}  // end namespace echo::numeric_array

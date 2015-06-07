#pragma once

#include <echo/k_array.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/numeric_array_accessor.h>
#include <echo/numeric_array/numeric_array_initializer.h>
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
          typename std::iterator_traits<Pointer>::value_type>,
      public NumericArrayConstAccessor<
          NumericArrayView<Pointer, Shape, Structure>,
          KArrayView<Pointer, Shape>, Shape, Structure>,
      public NumericArrayConstInitializer<
          NumericArrayView<Pointer, Shape, Structure>,
          iterator_traits::value_type<Pointer>, Shape, Structure> {
  using KArrayViewBase = KArrayView<Pointer, Shape>;
  using ExpressionTemplateAssignmentBase =
      expression_template::ExpressionTemplateConstAssignment<
          NumericArrayView, numeric_array_expression_tag,
          typename std::iterator_traits<Pointer>::value_type>;
  using AccessorBase =
      NumericArrayConstAccessor<NumericArrayView, KArrayView<Pointer, Shape>,
                                Shape, Structure>;

 public:
  using structure = Structure;
  using KArrayViewBase::operator=;
  using KArrayViewBase::KArrayViewBase;
  using ExpressionTemplateAssignmentBase::operator=;
  using AccessorBase::operator();

  CONCEPT_MEMBER_REQUIRES(echo::concept::writable<Pointer>())
  auto& operator=(InitializerMultilist<iterator_traits::value_type<Pointer>,
                                       shape_traits::num_dimensions<Shape>()>
                      values) const {
    this->initialize(values);
    return *this;
  }

  CONCEPT_MEMBER_REQUIRES(echo::concept::writable<Pointer>() &&
                          shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  auto& operator=(
      InitializerMultilist<iterator_traits::value_type<Pointer>, 1> values) {
    this->initialize(values);
    return *this;
  }
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

///////////////
// make_view //
///////////////

template <class Pointer, class Shape, class Structure>
auto make_view(const NumericArrayView<Pointer, Shape, Structure>& view) {
  return view;
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_view(const NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<Structure>(array.data(), array.shape());
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_view(NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<Structure>(array.data(), array.shape());
}

////////////////
// make_cview //
////////////////

template <class Pointer, class Shape, class Structure>
auto make_cview(const NumericArrayView<Pointer, Shape, Structure>& view) {
  return make_numeric_array_view<Structure>(view.const_data(), view.shape());
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_cview(
    const NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<Structure>(array.data(), array.shape());
}
}
}  // end namespace echo::numeric_array

#pragma once

#include <echo/numeric_array/numeric_array_view_fwd.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/numeric_array_accessor.h>
#include <echo/numeric_array/numeric_array_initializer.h>
#include <echo/execution_context.h>
#include <echo/k_array.h>
#include <echo/expression_template.h>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// NumericArrayView
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class Structure, class MemoryBackendTag>
class NumericArrayView
    : public expression_template::ExpressionTemplateConstAssignment<
          NumericArrayView<Pointer, Shape, Structure>,
          numeric_array_expression_tag,
          typename std::iterator_traits<Pointer>::value_type>,
      public NumericArrayConstAccessor<
          NumericArrayView<Pointer, Shape, Structure>, Shape, Structure>,
      public NumericArrayConstInitializer<
          NumericArrayView<Pointer, Shape, Structure>,
          iterator_traits::value_type<Pointer>, Shape, Structure> {
  using KArrayBase = KArrayView<Pointer, Shape, MemoryBackendTag>;
  using ExpressionTemplateAssignmentBase =
      expression_template::ExpressionTemplateConstAssignment<
          NumericArrayView, numeric_array_expression_tag,
          typename std::iterator_traits<Pointer>::value_type>;

 public:
  using structure = Structure;
  using ExpressionTemplateAssignmentBase::operator=;
  using memory_backend_tag = MemoryBackendTag;

  NumericArrayView(Pointer data, const Shape& shape) noexcept
      : _k_array(data, shape) {}

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

  auto data() { return _k_array.data(); }
  auto data() const { return _k_array.data(); }
  auto const_data() const { return _k_array.const_data(); }

  const auto& shape() const { return _k_array.shape(); }

  auto& k_array() { return _k_array; }
  auto& k_array() const { return _k_array; }

 private:
  KArrayBase _k_array;
};

//------------------------------------------------------------------------------
// make_numeric_array_view
//------------------------------------------------------------------------------
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

template <
    class Structure, class MemoryBackendTag, class Pointer, class Shape,
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     memory::concept::memory_backend_tag<MemoryBackendTag>() &&
                     echo::concept::contiguous_iterator<Pointer>() &&
                     k_array::concept::shape<Shape>())>
auto make_numeric_array_view(Pointer data, const Shape& shape) {
  return NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag>(data,
                                                                       shape);
}

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class Structure, class MemoryBackendTag>
auto make_view(
    const NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag>& view) {
  return view;
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_view(const NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<
      Structure, memory_backend_traits::memory_backend_tag<Allocator> >(
      array.data(), array.shape());
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_view(NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<
      Structure, memory_backend_traits::memory_backend_tag<Allocator> >(
      array.data(), array.shape());
}

//------------------------------------------------------------------------------
// make_cview
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class Structure, class MemoryBackendTag>
auto make_cview(
    const NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag>& view) {
  return make_numeric_array_view<Structure, MemoryBackendTag>(view.const_data(),
                                                              view.shape());
}

template <class Scalar, class Shape, class Structure, class Allocator>
auto make_cview(
    const NumericArray<Scalar, Shape, Structure, Allocator>& array) {
  return make_numeric_array_view<
      Structure, memory_backend_traits::memory_backend_tag<Allocator> >(
      array.data(), array.shape());
}
}
}  // end namespace echo::numeric_array

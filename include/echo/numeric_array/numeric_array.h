#pragma once

#define DETAIL_NS detail_numeric_array

#include <echo/numeric_array/numeric_array_fwd.h>
#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/structure.h>
#include <echo/numeric_array/numeric_array_accessor.h>
#include <echo/numeric_array/numeric_array_initializer.h>
#include <echo/numeric_array/copy.h>
#include <echo/execution_context.h>
#include <echo/expression_template.h>
#include <echo/memory.h>

namespace echo {
namespace numeric_array {

namespace DETAIL_NS {
//------------------------------------------------------------------------------
// NumericArrayBase
//------------------------------------------------------------------------------
template <class Indexes, class Scalar, class Shape, class Structure,
          class Allocator>
class NumericArrayBase {};

template <std::size_t... Indexes, class Scalar, class Shape, class Structure,
          class Allocator>
class NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                       Structure, Allocator>
    : public NumericArrayAccessor<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          Shape, Structure>,
      public NumericArrayInitializer<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          Scalar, Shape, Structure>

      {
  using KArrayBase = KArray<Scalar, Shape, Allocator>;

 public:
  using structure = Structure;
  using memory_backend_tag =
      memory_backend_traits::memory_backend_tag<Allocator>;

  explicit NumericArrayBase(const Allocator& allocator = Allocator())
      : _k_array(Shape(), allocator) {}

  explicit NumericArrayBase(const Shape& shape,
                            const Allocator& allocator = Allocator())
      : _k_array(shape, allocator) {}

  explicit NumericArrayBase(
      shape_traits::extent_type<Indexes, Shape>... extents,
      const Allocator& allocator = Allocator())
      : _k_array(make_shape(extents...), allocator) {
    static_assert(!echo::numeric_array::structure::concept::equal_dimensional<
                      Structure>(),
                  "");
  }

  CONCEPT_MEMBER_REQUIRES(
      echo::numeric_array::structure::concept::equal_dimensional<Structure>())
  explicit NumericArrayBase(shape_traits::extent_type<0, Shape> extent,
                            const Allocator& allocator = Allocator())
      : _k_array(make_shape((Indexes, extent)...), allocator) {}

  auto& operator=(InitializerMultilist<
      Scalar, shape_traits::num_dimensions<Shape>()> values) {
    this->initialize(values);
    return *this;
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() <= 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  auto& operator=(InitializerMultilist<Scalar, 1> values) {
    this->initialize(values);
    return *this;
  }

  Scalar* data() { return _k_array.data(); }
  const Scalar* data() const { return _k_array.data(); }
  const Scalar* const_data() const { return _k_array.const_data(); }

  const auto& shape() const { return _k_array.shape(); }

  auto& k_array() { return _k_array; }
  const auto& k_array() const { return _k_array; }

 private:
  KArrayBase _k_array;
};
}

//------------------------------------------------------------------------------
// NumericArray
//------------------------------------------------------------------------------
template <class Scalar, class Shape, class Structure, class Allocator>
class NumericArray
    : public DETAIL_NS::NumericArrayBase<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Scalar, Shape, Structure, Allocator>,
      public expression_template::ExpressionTemplateAssignment<
          NumericArray<Scalar, Shape, Structure, Allocator>,
          numeric_array_expression_tag, Scalar>

      {
 private:
  using Base = DETAIL_NS::NumericArrayBase<
      std::make_index_sequence<shape_traits::num_dimensions<Shape>()>, Scalar,
      Shape, Structure, Allocator>;
  using ExpressionTemplateAssignmentBase =
      expression_template::ExpressionTemplateAssignment<
          NumericArray<Scalar, Shape, Structure, Allocator>,
          numeric_array_expression_tag, Scalar>;

 public:
  using Base::Base;
  using Base::operator=;
  using ExpressionTemplateAssignmentBase::operator=;
};

//------------------------------------------------------------------------------
// StaticNumericArray
//------------------------------------------------------------------------------
template <class Scalar, class Shape, class Structure>
using StaticNumericArray =
    NumericArray<Scalar, Shape, Structure, memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// make_numeric_array
//------------------------------------------------------------------------------
template <class Scalar, class Structure, class... Extents,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           and_c<k_array::concept::extent<Extents>()...>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_numeric_array(const Dimensionality<Extents...>& dimensionality,
                        const Allocator& allocator = Allocator()) {
  auto shape = make_shape(dimensionality);
  return NumericArray<Scalar, decltype(shape), Structure, Allocator>(shape,
                                                                     allocator);
}

template <class Scalar, class... Extents,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           and_c<k_array::concept::extent<Extents>()...>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_numeric_array(const Dimensionality<Extents...>& dimensionality,
                        const Allocator& allocator = Allocator()) {
  auto shape = make_shape(dimensionality);
  return NumericArray<Scalar, decltype(shape),
                      execution_context::structure::general, Allocator>(
      shape, allocator);
}

template <
    class ExecutionContext, class X,
    CONCEPT_REQUIRES(
        execution_context::concept::expression_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        concept::numeric_array<uncvref_t<X>>())>
auto make_numeric_array(const ExecutionContext& execution_context, X&& x) {
  using Scalar = numeric_array_traits::value_type<uncvref_t<X>>;
  using Structure = numeric_array_traits::structure<uncvref_t<X>>;
  auto result = make_numeric_array<Scalar, Structure>(
      get_dimensionality(x), make_aligned_allocator<Scalar>(execution_context));
  copy(execution_context, x, result);
  return result;
}

//------------------------------------------------------------------------------
// make_static_numeric_array
//------------------------------------------------------------------------------
template <
    class Scalar, class Structure, class... Extents,
    CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                     execution_context::concept::structure<Structure>() &&
                     and_c<k_array::concept::static_extent<Extents>()...>())>
auto make_static_numeric_array(
    const Dimensionality<Extents...>& dimensionality) {
  auto shape = make_shape(dimensionality);
  return NumericArray<Scalar, decltype(shape), Structure,
                      memory::SimdStaticAllocator<Scalar>>(shape);
}
template <
    class Scalar, class... Extents,
    CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                     and_c<k_array::concept::static_extent<Extents>()...>())>
auto make_static_numeric_array(
    const Dimensionality<Extents...>& dimensionality) {
  auto shape = make_shape(dimensionality);
  return NumericArray<Scalar, decltype(shape),
                      execution_context::structure::general,
                      memory::SimdStaticAllocator<Scalar>>(shape);
}
template <class ExecutionContext, class X,
          CONCEPT_REQUIRES(execution_context::concept::expression_executer<
                               ExecutionContext>() &&
                           concept::numeric_array<uncvref_t<X>>() &&
                           k_array::concept::static_shape<
                               shaped_traits::shape_type<uncvref_t<X>>>())>
auto make_static_numeric_array(const ExecutionContext& execution_context,
                               X&& x) {
  using Scalar = numeric_array_traits::value_type<uncvref_t<X>>;
  using Structure = numeric_array_traits::structure<uncvref_t<X>>;
  auto result =
      make_static_numeric_array<Scalar, Structure>(get_dimensionality(x));
  copy(execution_context, x, result);
  return result;
}
}
}  // end namespace echo::numeric_array

#undef DETAIL_NS

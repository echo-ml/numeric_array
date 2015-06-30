#pragma once

#define DETAIL_NS detail_numeric_array

#include <echo/numeric_array/numeric_array_fwd.h>
#include <echo/k_array.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/structure.h>
#include <echo/numeric_array/numeric_array_accessor.h>
#include <echo/numeric_array/numeric_array_initializer.h>
#include <echo/execution_context.h>
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
      : KArrayBase(make_k_shape((Indexes, extent)...), allocator) {}

  auto& operator=(InitializerMultilist<
      Scalar, shape_traits::num_dimensions<Shape>()> values) {
    this->initialize(values);
    return *this;
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
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
  auto& k_array() const { return _k_array; }

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
}
}  // end namespace echo::numeric_array

#undef DETAIL_NS

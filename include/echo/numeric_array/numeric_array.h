#pragma once

#include <echo/k_array.h>
#include <echo/simd_allocator.h>
#include <echo/expression_template.h>

#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/structure.h>
#include <echo/numeric_array/numeric_array_accessor.h>
#include <echo/numeric_array/numeric_array_initializer.h>
#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

namespace detail {
namespace numeric_array {

//////////////////////
// NumericArrayBase //
//////////////////////

template <class Indexes, class Scalar, class Shape, class Structure,
          class Allocator>
class NumericArrayBase {};

template <std::size_t... Indexes, class Scalar, class Shape, class Structure,
          class Allocator>
class NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                       Structure, Allocator>
    : public KArray<Scalar, Shape, Allocator>,
      public expression_template::ExpressionTemplateAssignment<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          numeric_array_expression_tag, Scalar>,
      public KArrayAssignment<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          Scalar>,
      public NumericArrayAccessor<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          KArray<Scalar, Shape, Allocator>, Shape, Structure>,
      public NumericArrayInitializer<
          NumericArrayBase<std::index_sequence<Indexes...>, Scalar, Shape,
                           Structure, Allocator>,
          Scalar, Shape, Structure>

      {
  using KArrayBase = KArray<Scalar, Shape, Allocator>;
  using AssignmentBase = KArrayAssignment<NumericArrayBase, Scalar>;
  using ExpressionTemplateAssignmentBase =
      expression_template::ExpressionTemplateAssignment<
          NumericArrayBase, numeric_array_expression_tag, Scalar>;
  using AccessorBase =
      NumericArrayAccessor<NumericArrayBase, KArrayBase, Shape, Structure>;

 public:
  using structure = Structure;
  using KArrayBase::KArrayBase;
  using AssignmentBase::operator=;
  using ExpressionTemplateAssignmentBase::operator=;
  using AccessorBase::operator();

  explicit NumericArrayBase(const Allocator& allocator = Allocator())
      : KArrayBase(Shape(), allocator) {}

  explicit NumericArrayBase(
      shape_traits::extent_type<Indexes, Shape>... extents,
      const Allocator& allocator = Allocator())
      : KArrayBase(make_k_shape(extents...), allocator) {
    static_assert(!echo::numeric_array::structure::concept::equal_dimensional<
                      Structure>(),
                  "");
  }

  CONCEPT_MEMBER_REQUIRES(
      echo::numeric_array::structure::concept::equal_dimensional<Structure>())
  explicit NumericArrayBase(shape_traits::extent_type<0, Shape> extent,
                            const Allocator& allocator = Allocator())
      : KArrayBase(make_k_shape((Indexes, extent)...), allocator) {}

  // broken with intel's compiler //
  // CONCEPT_MEMBER_REQUIRES(
  //     shape_traits::num_free_dimensions<Shape>() == 1 &&
  //     shape_traits::num_dimensions<Shape>() != 1 &&
  //     !and_c<k_array::is_static_extent<Indexes, Shape>()...>())
  // explicit NumericArrayBase(index_t n, const Allocator& allocator =
  // Allocator())
  //     : KArrayBase(Shape(n), allocator) {}

  // CONCEPT_MEMBER_REQUIRES(
  //     and_c<k_array::is_static_extent<Indexes, Shape>()...>())
  // NumericArrayBase(InitializerMultilist<Scalar, sizeof...(Indexes)> values,
  //                  const Allocator& allocator = Allocator())
  //     : KArrayBase(Shape(), allocator) {
  //   this->initialize(values);
  // }
  //
  // CONCEPT_MEMBER_REQUIRES(
  //     and_c<!k_array::is_static_extent<Indexes, Shape>()...>())
  // NumericArrayBase(InitializerMultilist<Scalar, sizeof...(Indexes)> values,
  //                  const Allocator& allocator = Allocator())
  //     : KArrayBase(make_k_shape(std::get<Indexes>(
  //                      InitializerMultilistAccessor<Scalar,
  //                      sizeof...(Indexes)>(
  //                          values).extents())...),
  //                  allocator) {
  //   this->initialize(values);
  // }

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
};
}
}

//////////////////
// NumericArray //
//////////////////

template <class Scalar, class Shape,
          class Structure = execution_context::structure::general,
          class Allocator = SimdAllocator<Scalar>>
struct NumericArray
    : detail::numeric_array::NumericArrayBase<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Scalar, Shape, Structure, Allocator> {
 private:
  using Base = detail::numeric_array::NumericArrayBase<
      std::make_index_sequence<shape_traits::num_dimensions<Shape>()>, Scalar,
      Shape, Structure, Allocator>;

 public:
  using Base::Base;
  using Base::operator=;
};
}
}  // end namespace echo::numeric_array

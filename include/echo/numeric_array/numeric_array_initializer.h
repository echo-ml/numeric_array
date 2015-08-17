#pragma once

#define DETAIL_NS detail_numeric_array_initializer

#include <echo/utility/initializer_multilist.h>
#include <echo/k_array.h>
#include <stdexcept>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// InitializationError
//------------------------------------------------------------------------------
struct InitializationError : virtual std::runtime_error {
  InitializationError() : std::runtime_error("InitializationError") {}
};

//------------------------------------------------------------------------------
// initialize
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <int I, class Scalar, class Functor, class Shape,
          CONCEPT_REQUIRES(I == shape_traits::num_dimensions<Shape>())>
void initialize_impl(Scalar value, const Shape& shape, const Functor& functor) {
  functor(value);
}

template <int I, class Values, class Functor, class Shape,
          CONCEPT_REQUIRES(I != shape_traits::num_dimensions<Shape>())>
void initialize_impl(Values values, const Shape& shape,
                     const Functor& functor) {
  if (values.size() != get_extent<I>(shape)) throw InitializationError();
  index_t index = 0;
  for (auto values_i : values) {
    auto functor_new =
        [&](auto value, auto... indexes) { functor(value, index, indexes...); };
    initialize_impl<I + 1>(values_i, shape, functor_new);
    ++index;
  }
}

template <class Scalar, class Shape, class Derived>
void initialize(
    InitializerMultilist<Scalar, shape_traits::num_dimensions<Shape>()> values,
    const Shape& shape, Derived& derived) {
  initialize_impl<0>(values, shape, [&](auto value, auto... indexes) {
    derived(indexes...) = value;
  });
}
}

//------------------------------------------------------------------------------
// NumericArrayInitializer
//------------------------------------------------------------------------------
template <class Derived, class Scalar, class Shape, class Structure>
struct NumericArrayInitializer {
  void initialize(InitializerMultilist<
      Scalar, shape_traits::num_dimensions<Shape>()> values) {
    auto& derived = static_cast<Derived&>(*this);
    DETAIL_NS::initialize<Scalar>(values, derived.shape(), derived);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() <= 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  void initialize(InitializerMultilist<Scalar, 1> values) {
    auto& derived = static_cast<Derived&>(*this);
    for (int i = 0; i < get_num_elements(derived); ++i)
      derived(i) = *(std::begin(values) + i);
  }
};

//------------------------------------------------------------------------------
// NumericArrayConstInitializer
//------------------------------------------------------------------------------
template <class Derived, class Scalar, class Shape, class Structure>
struct NumericArrayConstInitializer
    : NumericArrayInitializer<Derived, Scalar, Shape, Structure> {
  void initialize(InitializerMultilist<
      Scalar, shape_traits::num_dimensions<Shape>()> values) const {
    auto mutable_this = const_cast<NumericArrayConstInitializer*>(this);
    mutable_this->NumericArrayInitializer<Derived, Scalar, Shape,
                                          Structure>::initialize(values);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() <= 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  void initialize(InitializerMultilist<Scalar, 1> values) const {
    auto mutable_this = const_cast<NumericArrayConstInitializer*>(this);
    mutable_this->NumericArrayInitializer<Derived, Scalar, Shape,
                                          Structure>::initialize(values);
  }
};
}
}

#undef DETAIL_NS

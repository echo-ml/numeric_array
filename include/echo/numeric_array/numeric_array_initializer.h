#pragma once

#include <echo/numeric_array/initializer.h>
#include <echo/k_array.h>
#include <stdexcept>

namespace echo {
namespace numeric_array {

/////////////////////////
// InitializationError //
/////////////////////////

struct InitializationError : virtual std::runtime_error {
  InitializationError() : std::runtime_error("InitializationError") {}
};

////////////////
// initialize //
////////////////

namespace detail {
namespace numeric_array_initializer {

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
    Initializer<Scalar, shape_traits::num_dimensions<Shape>()> values,
    const Shape& shape, Derived& derived) {
  initialize_impl<0>(values, shape, [&](Scalar value, auto... indexes) {
    derived(indexes...) = value;
  });
}
}
}

/////////////////////////////
// NumericArrayInitializer //
/////////////////////////////

template <class Derived, class Scalar, class Shape, class Structure>
struct NumericArrayInitializer {
  void initialize(
      Initializer<Scalar, shape_traits::num_dimensions<Shape>()> values) {
    auto& derived = static_cast<Derived&>(*this);
    detail::numeric_array_initializer::initialize<Scalar>(
        values, derived.shape(), derived);
  }
};

//////////////////////////////////
// NumericArrayConstInitializer //
//////////////////////////////////

template <class Derived, class Scalar, class Shape, class Structure>
struct NumericArrayConstInitializer
    : NumericArrayInitializer<Derived, Scalar, Shape, Structure> {
  void initialize(
      Initializer<Scalar, shape_traits::num_dimensions<Shape>()> values) const {
    auto mutable_this = const_cast<NumericArrayConstInitializer*>(this);
    mutable_this->NumericArrayInitializer<Derived, Scalar, Shape,
                                          Structure>::initialize(values);
  }
};
}
}
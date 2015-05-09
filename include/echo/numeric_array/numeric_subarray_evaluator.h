#pragma once

#include <echo/execution_context.h>
#include <echo/k_array.h>

namespace echo {
namespace numeric_array {

////////////////
// get_offset //
////////////////

namespace detail {
namespace numeric_subarray_evaluator {

template <class ShapeStrides,
          CONCEPT_REQUIRES(k_array::concept::shape_strides<ShapeStrides>())>
index_t get_offset(std::index_sequence<>, const ShapeStrides& shape_strides) {
  return 0;
}

template <std::size_t IFirst, std::size_t... IRest, class ShapeStrides,
          class... IndexesRest,
          CONCEPT_REQUIRES(k_array::concept::shape_strides<ShapeStrides>())>
index_t get_offset(std::index_sequence<IFirst, IRest...>,
                   const ShapeStrides& shape_strides, index_t index_first,
                   index_t index_first_size, IndexesRest... indexes_rest) {
  return get_stride<IFirst>(shape_strides) * index_first +
         get_offset(std::index_sequence<IRest...>(), shape_strides,
                    indexes_rest...);
}
}
}

//////////////////////////////////
// NumericSubarrayEvaluatorImpl //
//////////////////////////////////

namespace detail {
namespace numeric_subarray_evaluator {

template <class, class>
struct NumericSubarrayEvaluatorImpl {};

template <class Derived>
struct NumericSubarrayEvaluatorImpl<std::index_sequence<0>, Derived> {
  decltype(auto) operator()(index_t i) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return *(derived.data() + get_stride<0>(derived.shape_strides()) * i);
  }
};

template <std::size_t... Ix, class Derived>
struct NumericSubarrayEvaluatorImpl<std::index_sequence<Ix...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<Ix || true, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    index_t offset = get_offset(std::make_index_sequence<sizeof...(Ix) / 2>(),
                                derived.shape_strides(), indexes...);
    return *(derived.data() + offset);
  }
};
}
}

//////////////////////////////
// NumericSubarrayEvaluator //
//////////////////////////////

template <class Pointer, class ShapeStrides>
class NumericSubarrayEvaluator
    : ShapeStrides,
      public detail::numeric_subarray_evaluator::NumericSubarrayEvaluatorImpl<
          std::make_index_sequence<(ShapeStrides::StrideSequence::size > 1
                                        ? ShapeStrides::StrideSequence::size * 2
                                        : 1)>,
          NumericSubarrayEvaluator<Pointer, ShapeStrides> > {
 public:
  NumericSubarrayEvaluator(Pointer data, const ShapeStrides& shape_strides)
      : _data(data), ShapeStrides(shape_strides) {}
  auto data() const { return _data; }
  const auto& shape_strides() const {
    return static_cast<const ShapeStrides&>(*this);
  }

 private:
  Pointer _data;
};

template <class Pointer, class ShapeStrides,
          CONCEPT_REQUIRES(k_array::concept::shape_strides<ShapeStrides>())>
auto make_numeric_subarray_evaluator(Pointer data,
                                     const ShapeStrides& shape_strides) {
  return NumericSubarrayEvaluator<Pointer, ShapeStrides>(data, shape_strides);
}
}
}

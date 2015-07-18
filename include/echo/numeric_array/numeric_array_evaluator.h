#pragma once

#define DETAIL_NS detail_numeric_array_evaluator

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/evaluator.h>
#include <echo/execution_context.h>
#include <echo/repeat_type.h>
#include <echo/contract.h>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// ContiguousNumericArrayEvaluator
//------------------------------------------------------------------------------
template <class Pointer>
class ContiguousNumericArrayEvaluator {
  CONCEPT_ASSERT(echo::concept::contiguous_iterator<Pointer>() &&
                 execution_context::concept::scalar<
                     iterator_traits::value_type<Pointer>>());

 public:
  ContiguousNumericArrayEvaluator(Pointer data) : _data(data) {}
  decltype(auto) operator()(index_t index) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(_data != nullptr);
      CONTRACT_ASSERT(valid_evaluation(index));
    };
    return *(_data + index);
  }

 private:
  Pointer _data;
};

template <class Pointer,
          CONCEPT_REQUIRES(echo::concept::contiguous_iterator<Pointer>() &&
                           execution_context::concept::scalar<
                               iterator_traits::value_type<Pointer>>())>
auto make_contiguous_numeric_array_evaluator(Pointer data) {
  return ContiguousNumericArrayEvaluator<Pointer>(data);
}

//------------------------------------------------------------------------------
// get_subarray_offset
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Strides,
          CONCEPT_REQUIRES(k_array::concept::index_tuple<Strides>())>
index_t get_subarray_offset(std::index_sequence<>, const Strides& strides) {
  return 0;
}
template <std::size_t IFirst, std::size_t... IRest, class Strides,
          class... IndexesRest,
          CONCEPT_REQUIRES(k_array::concept::index_tuple<Strides>())>
index_t get_subarray_offset(std::index_sequence<IFirst, IRest...>,
                            const Strides& strides, index_t index_first,
                            index_t index_first_size,
                            IndexesRest... indexes_rest) {
  return htl::get<IFirst>(strides) * index_first +
         get_subarray_offset(std::index_sequence<IRest...>(), strides,
                             indexes_rest...);
}
}

//------------------------------------------------------------------------------
// NumericSubarrayEvaluator
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class, class>
struct NumericSubarrayEvaluatorImpl {};

template <class Derived>
struct NumericSubarrayEvaluatorImpl<std::index_sequence<0>, Derived> {
  decltype(auto) operator()(index_t i) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return *(derived.data() + htl::get<0>(derived.strides()) * i);
  }
};

template <std::size_t... Ix, class Derived>
struct NumericSubarrayEvaluatorImpl<std::index_sequence<Ix...>, Derived> {
  decltype(auto) operator()(repeat_type_c<Ix, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(derived.data() != nullptr);
      CONTRACT_ASSERT(valid_evaluation(indexes...));
    };
    index_t offset =
        get_subarray_offset(std::make_index_sequence<sizeof...(Ix) / 2>(),
                            derived.strides(), indexes...);
    return *(derived.data() + offset);
  }
};
}

template <class Pointer, class Strides>
class NumericSubarrayEvaluator
    : htl::Pack<Strides>,
      public DETAIL_NS::NumericSubarrayEvaluatorImpl<
          std::make_index_sequence<(
              htl::tuple_traits::num_elements<Strides>() > 1
                  ? htl::tuple_traits::num_elements<Strides>() * 2
                  : 1)>,
          NumericSubarrayEvaluator<Pointer, Strides>> {
 public:
  NumericSubarrayEvaluator(Pointer data, const Strides& strides)
      : htl::Pack<Strides>(strides), _data(data) {}
  auto data() const { return _data; }
  const auto& strides() const { return htl::unpack<Strides>(*this); }

 private:
  Pointer _data;
};

template <class Pointer, class Strides,
          CONCEPT_REQUIRES(k_array::concept::index_tuple<Strides>())>
auto make_numeric_subarray_evaluator(Pointer data, const Strides& strides) {
  return NumericSubarrayEvaluator<Pointer, Strides>(data, strides);
}

//------------------------------------------------------------------------------
// make_numeric_array_evaluator
//------------------------------------------------------------------------------
template <class NumericArray,
          CONCEPT_REQUIRES(
              concept::contiguous_numeric_array<uncvref_t<NumericArray>>())>
auto make_numeric_array_evaluator(NumericArray&& numeric_array) {
  return make_contiguous_numeric_array_evaluator(numeric_array.data());
}

template <class NumericArray,
          CONCEPT_REQUIRES(
              !concept::contiguous_numeric_array<uncvref_t<NumericArray>>() &&
              concept::numeric_array<uncvref_t<NumericArray>>())>
auto make_numeric_array_evaluator(NumericArray&& numeric_array) {
  return make_numeric_subarray_evaluator(numeric_array.data(),
                                         numeric_array.shape().strides());
}
}
}

#undef DETAIL_NS

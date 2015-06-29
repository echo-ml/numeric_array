#pragma once

#define DETAIL_NS detail_test

#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/map_indexes_expression.h>
#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <echo/test.h>
#include <echo/utility/initializer_multilist.h>
#include <sstream>

namespace echo {
namespace numeric_array {

namespace DETAIL_NS {

///////////////////
// extents_check //
///////////////////

template <int I, class Shape, class SizeType,
          CONCEPT_REQUIRES(k_array::concept::shape<Shape>() &&
                           I == shape_traits::num_dimensions<Shape>())>
void extents_check(
    const Shape& shape,
    const std::array<SizeType, shape_traits::num_dimensions<Shape>()>&
        extents) {}

template <int I, class Shape, class SizeType,
          CONCEPT_REQUIRES(k_array::concept::shape<Shape>() &&
                           I != shape_traits::num_dimensions<Shape>())>
void extents_check(
    const Shape& shape,
    const std::array<SizeType, shape_traits::num_dimensions<Shape>()>&
        extents) {
  INFO("dimension = " << I);
  CHECK(get_extent<I>(shape) == extents[I]);
}

/////////////////////
// apply_predicate //
/////////////////////

template <int I, class Shape, class Functor,
          CONCEPT_REQUIRES(k_array::concept::shape<Shape>() &&
                           I == shape_traits::num_dimensions<Shape>())>
void apply_predicate(const Shape& shape, const Functor& functor) {
  functor();
}

template <int I, class Shape, class Functor,
          CONCEPT_REQUIRES(k_array::concept::shape<Shape>() &&
                           I != shape_traits::num_dimensions<Shape>())>
void apply_predicate(const Shape& shape, const Functor& functor) {
  for (int i = 0; i < get_extent<I>(shape); ++i) {
    apply_predicate<I + 1>(shape,
                           [&](auto... indexes) { functor(i, indexes...); });
  }
}

/////////////////
// array_check //
/////////////////

template <class Scalar, int N, class Pointer, class Shape, class Structure,
          class MemoryBackendTag, class Predicate>
void array_check_impl(
    NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag> array1,
    InitializerMultilist<Scalar, N> values, const Predicate& predicate) {
  auto accessor = InitializerMultilistAccessor<Scalar, N>(values);
  const auto& shape = array1.shape();

  extents_check<0>(shape, accessor.extents());

  apply_predicate<0>(shape, [&](auto... indexes) {
    std::ostringstream oss;
    [](auto...) {}((oss << indexes << " ", 0)...);
    INFO("index = ( " << oss.str() << ")");
    predicate(array1(indexes...), accessor(indexes...));
  });
}

template <class Pointer, class Shape, class Structure>
void array_check(
    const NumericArrayView<Pointer, Shape, Structure>& array1,
    InitializerMultilist<uncvref_t<decltype(*std::declval<Pointer>())>,
                         shape_traits::num_dimensions<Shape>()> values,
    double tolerance = 0.0) {
  using Scalar = uncvref_t<decltype(*std::declval<Pointer>())>;
  constexpr int N = shape_traits::num_dimensions<Shape>();
  auto accessor = InitializerMultilistAccessor<Scalar, N>(values);
  const auto& shape = array1.shape();
  array_check_impl<Scalar, N>(array1, values, [=](auto x, auto y) {
    if (tolerance == 0.0)
      CHECK(x == y);
    else
      CHECK(x == Approx(y).epsilon(tolerance));
  });
}

template <class Pointer, class Shape, class Structure, class MemoryBackendTag>
void array_check(
    InitializerMultilist<uncvref_t<decltype(*std::declval<Pointer>())>,
                         shape_traits::num_dimensions<Shape>()> values,
    const NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag>& array1,
    double tolerance = 0.0) {
  array_check(array1, values, tolerance);
}

template <class Scalar, class Shape, class Structure, class Allocator>
void array_check(
    const NumericArray<Scalar, Shape, Structure, Allocator>& array1,
    InitializerMultilist<Scalar, shape_traits::num_dimensions<Shape>()> values,
    double tolerance = 0.0) {
  array_check(make_cview(array1), values, tolerance);
}

template <class Scalar, class Shape, class Structure, class Allocator>
void array_check(
    InitializerMultilist<Scalar, shape_traits::num_dimensions<Shape>()> values,
    const NumericArray<Scalar, Shape, Structure, Allocator>& array1,
    double tolerance = 0.0) {
  array_check(make_cview(array1), values, tolerance);
}
}
}
}

#define ARRAY_EQUAL(...) \
  echo::numeric_array::detail_test::array_check(__VA_ARGS__)

#undef DETAIL_NS

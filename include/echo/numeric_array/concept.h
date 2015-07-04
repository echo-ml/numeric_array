#pragma once

#define DETAIL_NS detail_concept

#include <echo/numeric_array/numeric_array_fwd.h>
#include <echo/numeric_array/numeric_array_view_fwd.h>
#include <echo/numeric_array/structure_traits.h>
#include <echo/numeric_array/numeric_array_traits.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/null_dimensionality.h>
#include <echo/execution_context.h>
#include <echo/repeat_type.h>

namespace echo {
namespace numeric_array {
namespace concept {

//------------------------------------------------------------------------------
// numeric_array_deep
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Scalar, class Shape, class Structure, class Allocator>
auto numeric_array_impl(NumericArray<Scalar, Shape, Structure, Allocator> && )
    -> std::true_type;

template <class T>
auto numeric_array_impl(T && ) -> std::false_type;
}

template <class T>
constexpr bool numeric_array_deep() {
  using Result = decltype(DETAIL_NS::numeric_array_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// numeric_array_view
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Pointer, class Shape, class Structure, class MemoryBackendTag>
auto numeric_array_view_impl(
    NumericArrayView<Pointer, Shape, Structure, MemoryBackendTag> && )
    -> std::true_type;

template <class T>
auto numeric_array_view_impl(T && ) -> std::false_type;
}

template <class T>
constexpr bool numeric_array_view() {
  using Result =
      decltype(DETAIL_NS::numeric_array_view_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// numeric_array
//------------------------------------------------------------------------------
template <class T>
constexpr bool numeric_array() {
  return numeric_array_deep<T>() || numeric_array_view<T>();
}

namespace DETAIL_NS {
template <class Structure>
struct StructuredNumericArray : Concept {
  template <class T>
  auto require(T&& x) -> list<numeric_array<T>(),
                              convertible<typename T::structure, Structure>()>;
};
}

template <class Structure, class T>
constexpr bool numeric_array() {
  return models<DETAIL_NS::StructuredNumericArray<Structure>, T>();
}

namespace DETAIL_NS {
template <int K>
struct KNumericArray : Concept {
  template <class T>
  auto require(T&& numeric_array) -> list<
      concept::numeric_array<T>(),
      k_array::concept::shape<K, uncvref_t<decltype(numeric_array.shape())>>()>;
};
}

template <int K, class T>
constexpr bool numeric_array() {
  return models<DETAIL_NS::KNumericArray<K>, T>();
}

//------------------------------------------------------------------------------
// modifiable_numeric_array_forward
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ModifiableNumericArrayForward : Concept {
  template <class T>
  auto require(T&& x) -> list<numeric_array<uncvref_t<T>>(),
                              echo::concept::writable<decltype(x.data())>()>;
};
}

template <class T>
constexpr bool modifiable_numeric_array_forward() {
  return models<DETAIL_NS::ModifiableNumericArrayForward, T>();
}

//------------------------------------------------------------------------------
// contiguous_numeric_array
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ContiguousNumericArray : Concept {
  template <class T>
  auto require(T&& a) -> list<
      numeric_array<T>(),
      k_array::concept::contiguous_shape<uncvref_t<decltype(a.shape())>>()>;
};
}

template <class T>
constexpr bool contiguous_numeric_array() {
  return models<DETAIL_NS::ContiguousNumericArray, T>();
}

template <class Structure, class T>
constexpr bool contiguous_numeric_array() {
  return contiguous_numeric_array<T>() && numeric_array<Structure, T>();
}

//------------------------------------------------------------------------------
// dimensioned_expression
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct DimensionedExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<execution_context::concept::expression<T>(),
                              same<typename T::expression_template_tag,
                                   numeric_array_expression_tag>()>;
};
}

template <class T>
constexpr bool dimensioned_expression() {
  return models<DETAIL_NS::DimensionedExpression, T>();
}

namespace DETAIL_NS {
template <int K>
struct KDimensionedExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<dimensioned_expression<T>(),
                              dimensionality_traits::num_dimensions<
                                  uncvref_t<decltype(x.shape())>>() == K>;
};
}

template <int K, class T>
constexpr bool dimensioned_expression() {
  return models<DETAIL_NS::KDimensionedExpression<K>, T>();
}

//------------------------------------------------------------------------------
// scalar_expression
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ScalarExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<
      same<typename T::expression_template_tag, numeric_array_expression_tag>(),
      same<expression_traits::structure<T>, structure::scalar>(),
      execution_context::concept::flat_evaluator<
          uncvref_t<decltype(x.evaluator())>>(),
      same<uncvref_t<decltype(x.dimensionality())>, NullDimensionality>()>;
};
}

template <class T>
constexpr bool scalar_expression() {
  return models<DETAIL_NS::ScalarExpression, T>();
}

//------------------------------------------------------------------------------
// expression
//------------------------------------------------------------------------------
template <class T>
constexpr bool expression() {
  return scalar_expression<T>() || dimensioned_expression<T>();
}

//------------------------------------------------------------------------------
// compatible_structures
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct CompatibleStructures : Concept {
  template <class... Tx>
  auto require(Tx&&... tx) -> list<
      execution_context::concept::structure<structure_traits::fuse<Tx...>>()>;
};
}

template <class... Tx>
constexpr bool compatible_structures() {
  return models<DETAIL_NS::CompatibleStructures, Tx...>();
}

//------------------------------------------------------------------------------
// structure_convertible_to
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct StructureConvertibleTo : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b) -> list<same<structure_traits::fuse<A, B>, B>()>;
};
}

template <class A, class B>
constexpr bool structure_convertible_to() {
  return models<DETAIL_NS::StructureConvertibleTo, A, B>();
}

//------------------------------------------------------------------------------
// compatible_numeric_arrays
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct CompatibleNumericArrays : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b) -> list<
      k_array::concept::compatible_k_arrays<uncvref_t<decltype(a.k_array())>,
                                            uncvref_t<decltype(b.k_array())>>(),
      compatible_structures<numeric_array_traits::structure<A>,
                            numeric_array_traits::structure<B>>()>;
};
}

template <class A, class B>
constexpr bool compatible_numeric_arrays() {
  return models<DETAIL_NS::CompatibleNumericArrays, A, B>();
}

//------------------------------------------------------------------------------
// compatible_expressions
//------------------------------------------------------------------------------
namespace DETAIL_NS {

template <bool, class... Expressions>
struct first_dimensioned_expression_impl_ {};

template <class ExpressionFirst, class... ExpressionsRest>
struct first_dimensioned_expression_impl_<true, ExpressionFirst,
                                          ExpressionsRest...> {
  using type = ExpressionFirst;
};

template <class ExpressionFirst, class ExpressionSecond,
          class... ExpressionsRest>
struct first_dimensioned_expression_impl_<false, ExpressionFirst,
                                          ExpressionSecond, ExpressionsRest...>
    : first_dimensioned_expression_impl_<
          dimensioned_expression<ExpressionSecond>(), ExpressionSecond,
          ExpressionsRest...> {};

template <class... Expressions>
struct first_dimensioned_expression_impl {};

template <class ExpressionFirst, class... ExpressionsRest>
struct first_dimensioned_expression_impl<ExpressionFirst, ExpressionsRest...>
    : first_dimensioned_expression_impl_<
          dimensioned_expression<ExpressionFirst>(), ExpressionFirst,
          ExpressionsRest...> {};

template <class... Expressions>
using first_dimensioned_expression =
    typename first_dimensioned_expression_impl<Expressions...>::type;

template <class Dimensionality, class... Expressions>
auto compatible_expression_dimensionalities_impl(int) -> std::integral_constant<
    bool, and_c<k_array::concept::compatible_dimensionalities<
                    Dimensionality,
                    expression_traits::dimensionality_type<Expressions>>() ||
                scalar_expression<Expressions>()...>()>;

template <class Dimensionality, class... Expressions>
auto compatible_expression_dimensionalities_impl(...) -> std::false_type;

template <class Dimensionality, class... Expressions>
constexpr bool compatible_expression_dimensionalities() {
  using Result =
      decltype(compatible_expression_dimensionalities_impl<Dimensionality,
                                                           Expressions...>(0));
  return Result::value;
}

struct CompatibleExpressions : Concept {
  template <class... Tx>
  auto require(Tx&&... tx)
      -> list<and_c<expression<Tx>()...>(),
              compatible_structures<expression_traits::structure<Tx>...>(),
              compatible_expression_dimensionalities<
                  expression_traits::dimensionality_type<
                      first_dimensioned_expression<Tx...>>,
                  Tx...>()>;
};
}

template <class... Tx>
constexpr bool compatible_expressions() {
  return models<DETAIL_NS::CompatibleExpressions, Tx...>();
}

//------------------------------------------------------------------------------
// compatible_functor_evaluators
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct CompatibleFunctorEvaluators : Concept {
  template <class Functor, class... Evaluators>
  auto require(Functor&& functor, Evaluators&&... evaluators)
      -> list<execution_context::concept::scalar<
          uncvref_t<decltype(functor(std::declval<
              type_traits::functor_return_type<Evaluators>>()...))>>()>;
};
}

template <class Functor, class... Evaluators>
constexpr bool compatible_functor_evaluators() {
  return models<DETAIL_NS::CompatibleFunctorEvaluators, Functor,
                Evaluators...>();
}

//------------------------------------------------------------------------------
// index_functor
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class>
struct IndexFunctor {};

template <std::size_t... Ix>
struct IndexFunctor<std::index_sequence<Ix...>> : Concept {
  template <class T>
  auto require(T&& functor) -> list<execution_context::concept::scalar<
      std::result_of_t<const T(repeat_type_c<Ix, index_t>...)>>()>;
};
}

template <int K, class Functor>
constexpr bool index_functor() {
  return models<DETAIL_NS::IndexFunctor<std::make_index_sequence<K>>,
                Functor>();
}
}
}
}

#undef DETAIL_NS

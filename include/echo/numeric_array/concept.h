#pragma once

#include <echo/numeric_array/structure_traits.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/null_shape.h>
#include <echo/execution_context.h>
// #include <echo/expression_template.h>

namespace echo {
namespace numeric_array {
namespace concept {

///////////////////
// numeric_array //
///////////////////

namespace detail {
namespace concept {
struct NumericArray : Concept {
  template <class T>
  auto require(T&& x) -> list<
      k_array::concept::k_array<T>(),
      execution_context::concept::scalar<k_array_traits::value_type<T>>(),
      execution_context::concept::structure<typename T::structure>(),
      same<typename T::expression_template_tag,
           numeric_array_expression_tag>()>;
};
}
}

template <class T>
constexpr bool numeric_array() {
  return models<detail::concept::NumericArray, T>();
}

namespace detail {
namespace concept {
template <class Structure>
struct StructuredNumericArray : Concept {
  template <class T>
  auto require(T&& x)
      -> list<numeric_array<T>(), same<typename T::structure, Structure>()>;
};
}
}

template <class Structure, class T>
constexpr bool numeric_array() {
  return models<detail::concept::StructuredNumericArray<Structure>, T>();
}

//////////////////////////////
// contiguous_numeric_array //
//////////////////////////////

template <class T>
constexpr bool contiguous_numeric_array() {
  return numeric_array<T>() && k_array::concept::contiguous_k_array<T>();
}

template <class Structure, class T>
constexpr bool contiguous_numeric_array() {
  return contiguous_numeric_array<T>() && numeric_array<Structure, T>();
}

///////////////////////
// shaped_expression //
///////////////////////

namespace detail {
namespace concept {
struct ShapedExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<execution_context::concept::expression<T>(),
                              same<typename T::expression_template_tag,
                                   numeric_array_expression_tag>()>;
};
}
}

template <class T>
constexpr bool shaped_expression() {
  return models<detail::concept::ShapedExpression, T>();
}

namespace detail {
namespace concept {
template <int K>
struct KShapedExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<
      shaped_expression<T>(),
      shape_traits::num_dimensions<uncvref_t<decltype(x.shape())>>() == K>;
};
}
}

template <int K, class T>
constexpr bool shaped_expression() {
  return models<detail::concept::KShapedExpression<K>, T>();
}

///////////////////////
// scalar_expression //
///////////////////////

namespace detail {
namespace concept {
struct ScalarExpression : Concept {
  template <class T>
  auto require(T&& x) -> list<
      same<typename T::expression_template_tag, numeric_array_expression_tag>(),
      same<expression_traits::structure<T>, structure::scalar>(),
      execution_context::concept::flat_evaluator<
          uncvref_t<decltype(x.evaluator())>>(),
      same<uncvref_t<decltype(x.shape())>, NullShape>()>;
};
}
}

template <class T>
constexpr bool scalar_expression() {
  return models<detail::concept::ScalarExpression, T>();
}

////////////////
// expression //
////////////////

template <class T>
constexpr bool expression() {
  return scalar_expression<T>() || shaped_expression<T>();
}

///////////////////////////
// compatible_structures //
///////////////////////////

namespace detail {
namespace concept {
struct CompatibleStructures : Concept {
  template <class... Tx>
  auto require(Tx&&... tx) -> list<
      execution_context::concept::structure<structure_traits::fuse_t<Tx...>>()>;
};
}
}

template <class... Tx>
constexpr bool compatible_structures() {
  return models<detail::concept::CompatibleStructures, Tx...>();
}

//////////////////////////////
// structure_convertible_to //
//////////////////////////////

namespace detail {
namespace concept {
struct StructureConvertibleTo : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b) -> list<same<structure_traits::fuse_t<A, B>, B>()>;
};
}
}

template <class A, class B>
constexpr bool structure_convertible_to() {
  return models<detail::concept::StructureConvertibleTo, A, B>();
}

////////////////////////////
// compatible_expressions //
////////////////////////////

namespace detail {
namespace concept {

template <bool, class... Expressions>
struct first_shaped_expression_impl {};

template <class ExpressionFirst, class... ExpressionsRest>
struct first_shaped_expression_impl<true, ExpressionFirst, ExpressionsRest...> {
  using type = ExpressionFirst;
};

template <class ExpressionFirst, class ExpressionSecond,
          class... ExpressionsRest>
struct first_shaped_expression_impl<false, ExpressionFirst, ExpressionSecond,
                                    ExpressionsRest...>
    : first_shaped_expression_impl<shaped_expression<ExpressionSecond>(),
                                   ExpressionSecond, ExpressionsRest...> {};

template <class... Expressions>
struct first_shaped_expression_impl2 {};

template <class ExpressionFirst, class... ExpressionsRest>
struct first_shaped_expression_impl2<ExpressionFirst, ExpressionsRest...>
    : first_shaped_expression_impl<shaped_expression<ExpressionFirst>(),
                                   ExpressionFirst, ExpressionsRest...> {};

template <class... Expressions>
using first_shaped_expression =
    typename first_shaped_expression_impl2<Expressions...>::type;

template <class Shape, class... Expressions>
auto compatible_expression_shapes_impl(int) -> std::integral_constant<
    bool,
    const_algorithm::and_c<std::is_same<Shape, expression_traits::shape_type<
                                                   Expressions>>::value ||
                           scalar_expression<Expressions>()...>()>;

template <class Shape, class... Expressions>
auto compatible_expression_shapes_impl(...) -> std::false_type;

template <class Shape, class... Expressions>
constexpr bool compatible_expression_shapes() {
  using Result =
      decltype(compatible_expression_shapes_impl<Shape, Expressions...>(0));
  return Result::value;
}

struct CompatibleExpressions : Concept {
  template <class... Tx>
  auto require(Tx&&... tx) -> list<
      const_algorithm::and_c<expression<Tx>()...>(),
      compatible_structures<expression_traits::structure<Tx>...>(),
      compatible_expression_shapes<
          expression_traits::shape_type<first_shaped_expression<Tx...>>,
          Tx...>()
      // this version doesn't work with intel compiler
      // const_algorithm::and_c<
      //     same<expression_traits::shape_type<first_shaped_expression<Tx...>>,
      //          expression_traits::shape_type<Tx>>() ||
      //     scalar_expression<Tx>()...>()
      >;
};
}
}

template <class... Tx>
constexpr bool compatible_expressions() {
  return models<detail::concept::CompatibleExpressions, Tx...>();
}

///////////////////////////////////
// compatible_functor_evaluators //
///////////////////////////////////

namespace detail {
namespace concept {
struct CompatibleFunctorEvaluators : Concept {
  template <class Functor, class... Evaluators>
  auto require(Functor&& functor, Evaluators&&... evaluators)
      -> list<execution_context::concept::scalar<
          uncvref_t<decltype(functor(std::declval<
              type_traits::functor_return_type<Evaluators>>()...))>>()>;
};
}
}

template <class Functor, class... Evaluators>
constexpr bool compatible_functor_evaluators() {
  return models<detail::concept::CompatibleFunctorEvaluators, Functor,
                Evaluators...>();
}

///////////////////
// index_functor //
///////////////////

namespace detail {
namespace concept {

template <class>
struct IndexFunctor {};

// template<>
// struct IndexFunctor<std::index_sequence<1>> : Concept {
//   template<class T>
//   auto require(T&& functor) -> list<
//     execution_context::concept::scalar<
//       std::result_of_t<const T(index_t)>
//     >()
//   >;
// };

template <std::size_t... Ix>
struct IndexFunctor<std::index_sequence<Ix...>> : Concept {
  template <class T>
  auto require(T&& functor) -> list<execution_context::concept::scalar<
      std::result_of_t<const T(std::enable_if_t<Ix || true, index_t>...)>>()>;
};
}
}

template <int K, class Functor>
constexpr bool index_functor() {
  return models<detail::concept::IndexFunctor<std::make_index_sequence<K>>,
                Functor>();
}
}
}
}

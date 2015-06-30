#pragma once

#define DETAIL_NS detail_structure

#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

namespace structure_traits {

//------------------------------------------------------------------------------
// merge
//------------------------------------------------------------------------------
template <class A, class B>
struct merge {};
}

namespace structure {

struct scalar : execution_context::structure::base {};

struct equal_dimensional : execution_context::structure::base {};

namespace concept {

//------------------------------------------------------------------------------
// scalar
//------------------------------------------------------------------------------
template <class T>
constexpr bool scalar() {
  return std::is_convertible<T, structure::scalar>::value;
}

//------------------------------------------------------------------------------
// equal_dimensional
//------------------------------------------------------------------------------
template <class T>
constexpr bool equal_dimensional() {
  return std::is_convertible<T, structure::equal_dimensional>::value;
}

//------------------------------------------------------------------------------
// mergeable
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct Mergeable : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b) -> list<execution_context::concept::structure<
      typename structure_traits::merge<A, B>::type>()>;
};
}

template <class A, class B>
constexpr bool mergeable() {
  return models<DETAIL_NS::Mergeable, A, B>();
}
}
};
}
}

#undef DETAIL_NS

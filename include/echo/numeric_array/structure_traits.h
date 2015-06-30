#pragma once

#define DETAIL_NS detail_structure_traits

#include <echo/numeric_array/structure.h>

namespace echo {
namespace numeric_array {
namespace structure_traits {

//------------------------------------------------------------------------------
// merge
//------------------------------------------------------------------------------
template <class T>
struct merge<T, structure::scalar> {
  using type = T;
};

//------------------------------------------------------------------------------
// fuse
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class T>
auto fuse_impl(T) -> T;

template <class T>
auto fuse_impl(T, T) -> T;

template <class A, class B,
          CONCEPT_REQUIRES(structure::concept::mergeable<A, B>() &&
                           !structure::concept::mergeable<B, A>())>
auto fuse_impl(A, B) -> typename merge<A, B>::type;

template <class A, class B,
          CONCEPT_REQUIRES(structure::concept::mergeable<B, A>() &&
                           !structure::concept::mergeable<A, B>())>
auto fuse_impl(A, B) -> typename merge<B, A>::type;

template <class A, class B,
          CONCEPT_REQUIRES(structure::concept::mergeable<A, B>() &&
                           structure::concept::mergeable<B, A>() &&
                           std::is_same<typename merge<A, B>::type,
                                        typename merge<B, A>::type>::value)>
auto fuse_impl(A, B) -> typename merge<A, B>::type;

template <class A, class B, class... Rest>
auto fuse_impl(A a, B b,
               Rest... rest) -> decltype(fuse_impl(fuse_impl(a, b), rest...));
}

template <class... Structures>
using fuse = decltype(DETAIL_NS::fuse_impl(std::declval<Structures>()...));
}
}
}

#undef DETAIL_NS

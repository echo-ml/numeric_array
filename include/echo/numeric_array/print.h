#pragma once

#define DETAIL_NS detail_print

#include <echo/numeric_array/concept.h>
#include <iostream>
#include <sstream>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// print_impl
//------------------------------------------------------------------------------
namespace DETAIL_NS {

template <class Accessor, class Extent>
void print_impl(std::ostream& out, const Accessor& accessor,
                const htl::Tuple<Extent>& extent) {
  auto n = htl::get<0>(extent);
  out << '{';
  for (index_t i = 0; i < n; ++i) {
    out << accessor(i);
    if (i != n - 1) out << ", ";
  }
  out << '}';
}

template <class Accessor, class... Extents>
void print_impl(std::ostream& out, const Accessor& accessor,
                const htl::Tuple<Extents...>& extents) {
  auto n = htl::get<0>(extents);
  auto extents_rest = htl::tail(extents);
  out << '{';
  for (index_t i = 0; i < n; ++i) {
    auto accessor_new =
        [&](auto... indexes_rest) { return accessor(i, indexes_rest...); };
    print_impl(out, accessor_new, extents_rest);
    if (i != n - 1) out << ", ";
  }
  out << '}';
}
}

//------------------------------------------------------------------------------
// operator<<
//------------------------------------------------------------------------------
template <class Array, CONCEPT_REQUIRES(concept::numeric_array<Array>())>
std::ostream& operator<<(std::ostream& out, const Array& array) {
  DETAIL_NS::print_impl(out, array, array.shape().extents());
  return out;
}

//------------------------------------------------------------------------------
// to_string
//------------------------------------------------------------------------------
template <class Array, CONCEPT_REQUIRES(concept::numeric_array<Array>())>
std::string to_string(const Array& array) {
  std::ostringstream oss;
  oss << array;
  return oss.str();
}
}
}

#undef DETAIL_NS

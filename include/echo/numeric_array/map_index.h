#pragma once

#include <echo/numeric_array/map_expression.h>

namespace echo { namespace numeric_array {

template<class Functor, class... Extents>
decltype(auto) map_index(const Functor& functor, Extents... extents) {
}

template<class Functor, class Shape>
decltype(auto) map_index(const Functor& functor, const Shape& shape) {
}

}}

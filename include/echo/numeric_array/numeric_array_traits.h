#pragma once

#include <echo/k_array.h>

namespace echo {
namespace numeric_array_traits {

//------------------------------------------------------------------------------
// structure
//------------------------------------------------------------------------------
template <class NumericArray>
using structure = typename uncvref_t<NumericArray>::structure;

//------------------------------------------------------------------------------
// value_type
//------------------------------------------------------------------------------
template <class NumericArray>
using value_type = uncvref_t<decltype(*std::declval<NumericArray>().data())>;

//------------------------------------------------------------------------------
// memory_backend_tag
//------------------------------------------------------------------------------
template <class NumericArray>
using memory_backend_tag = typename NumericArray::memory_backend_tag;

//------------------------------------------------------------------------------
// extent_type
//------------------------------------------------------------------------------
template <int I, class A>
using extent_type = decltype(get_extent<I>(std::declval<A>()));
}
}

#pragma once

#include <echo/k_array.h>

namespace echo { namespace numeric_array_traits {

template<class NumericArray>
using structure = typename uncvref_t<NumericArray>::structure;

}}

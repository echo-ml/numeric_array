#pragma once

#include <echo/memory.h>
#include <echo/execution_context.h>

namespace echo { namespace numeric_array {

template <class Scalar, class Shape,
          class Structure = execution_context::structure::general,
          class Allocator = memory::SimdAllocator<Scalar>>
class NumericArray;

}}

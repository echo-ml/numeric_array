#pragma once

#include <echo/execution_context.h>
#include <echo/memory.h>

namespace echo { namespace numeric_array {

template<class Pointer, class Shape,
  class Structure = execution_context::structure::general,
  class MemoryBackendTag = echo::memory::memory_backend_tag>
class NumericArrayView;

}}

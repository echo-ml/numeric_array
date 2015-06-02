#pragma once

#include <echo/numeric_array/concept.h>

namespace echo { namespace numeric_array {

template<class Array, 
  CONCEPT_REQUIRES(concept::numeric_array<Array>())
>
void print(const Array& array) {
}

}}

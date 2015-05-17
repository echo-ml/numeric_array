#pragma once

#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {
namespace structure {

struct scalar : execution_context::structure::base {};

struct equal_dimensional : execution_context::structure::base {};

namespace concept {
template <class T>
constexpr bool scalar() {
  return std::is_convertible<T, structure::scalar>::value;
}

template <class T>
constexpr bool equal_dimensional() {
  return std::is_convertible<T, structure::equal_dimensional>::value;
}
}
};
}
}

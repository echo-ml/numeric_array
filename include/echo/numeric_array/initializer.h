#pragma once

#include <initializer_list>

namespace echo {
namespace numeric_array {

/////////////////
// Initializer //
/////////////////

namespace detail {
namespace initializer {
template <class Scalar, int N>
struct InitializerImpl {
  using type =
      std::initializer_list<typename InitializerImpl<Scalar, N - 1>::type>;
};

template <class Scalar>
struct InitializerImpl<Scalar, 0> {
  using type = Scalar;
};
}
}

template <class Scalar, int N>
using Initializer =
    typename detail::initializer::InitializerImpl<Scalar, N>::type;
}
}

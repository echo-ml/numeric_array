#pragma once

#define DETAIL_NS detail_evaluator

#include <echo/index.h>
#include <echo/concept.h>

namespace echo {
namespace numeric_array {

inline bool valid_evaluation() { return true; }

inline bool valid_evaluation(index_t i) { return i >= 0; }

template <class... IndexesRest,
          CONCEPT_REQUIRES(
              and_c<std::is_convertible<IndexesRest, index_t>::value...>())>
inline bool valid_evaluation(index_t i, index_t n,
                             IndexesRest... indexes_rest) {
  return (i >= 0) && (i < n) && valid_evaluation(indexes_rest...);
}
}
}

#undef DETAIL_NS

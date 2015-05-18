#pragma once

#include <echo/execution_context.h>

namespace echo {
namespace numeric_array {

//////////////////////
// FlattenEvaluator //
//////////////////////

namespace detail {
namespace flatten_evaluator {

inline index_t flatten_indexes(index_t index, index_t size) { return index; }

template <class... IndexesRest>
index_t flatten_indexes(index_t index, index_t size,
                        IndexesRest... indexes_rest) {
  return index + size * flatten_indexes(indexes_rest...);
}

template <class Indexes, class Derived>
struct FlattenEvaluatorImpl {};

template <std::size_t... Indexes, class Derived>
struct FlattenEvaluatorImpl<std::index_sequence<Indexes...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.evaluator()(flatten_indexes(indexes...));
  }
};
}
}

template <int K, class Evaluator>
class FlattenEvaluator
    : public detail::flatten_evaluator::FlattenEvaluatorImpl<
          std::make_index_sequence<2 * K>, FlattenEvaluator<K, Evaluator> > {
  CONCEPT_ASSERT(execution_context::concept::flat_evaluator<Evaluator>());

 public:
  FlattenEvaluator(const Evaluator& evaluator) : _evaluator(evaluator) {}
  const Evaluator& evaluator() const { return _evaluator; }

 private:
  Evaluator _evaluator;
};

template <
    int K, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::flat_evaluator<Evaluator>())>
auto make_flatten_evaluator(const Evaluator& evaluator) {
  return FlattenEvaluator<K, Evaluator>(evaluator);
}

/////////////////////////////
// make_k_shaped_evaluator //
/////////////////////////////

template <
    int K, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::flat_evaluator<Evaluator>())>
auto make_k_shaped_evaluator(const Evaluator& evaluator) {
  return make_flatten_evaluator<K>(evaluator);
}

template <int K, class Evaluator,
          CONCEPT_REQUIRES(
              execution_context::concept::k_shaped_evaluator<K, Evaluator>())>
auto make_k_shaped_evaluator(const Evaluator& evaluator) {
  return evaluator;
}
}
}

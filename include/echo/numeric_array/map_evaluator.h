#pragma once

#include <echo/utility.h>
#include <echo/index.h>
#include <echo/const_algorithm.h>
#include <echo/type_traits.h>
#include <echo/concept2.h>
#include <echo/numeric_array/fuse_evaluator.h>
#include <echo/numeric_array/concept.h>

namespace echo {
namespace numeric_array {

namespace detail {
namespace map_evaluator {

template <class, class>
struct MapEvaluatorImpl {};

template <std::size_t... Ix, class Derived>
struct MapEvaluatorImpl<std::index_sequence<Ix...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<Ix || true, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return map_apply(derived.functor(), derived.evaluators(), indexes...);
  }
};
}
}

template <class Functor, class EvaluatorFirst, class... EvaluatorsRest>
class MapEvaluator
    : public detail::map_evaluator::MapEvaluatorImpl<
          std::make_index_sequence<
              type_traits::functor_arity<EvaluatorFirst>()>,
          MapEvaluator<Functor, EvaluatorFirst, EvaluatorsRest...>> {
 public:
  MapEvaluator(const Functor& function, const EvaluatorFirst& evaluator_first,
               const EvaluatorsRest&... evaluators_rest)
      : _functor(function), _evaluators(evaluator_first, evaluators_rest...) {}
  const auto& functor() const { return _functor; }
  const auto& evaluators() const { return _evaluators; }

 private:
  Functor _functor;
  std::tuple<EvaluatorFirst, EvaluatorsRest...> _evaluators;
};

namespace detail {
namespace map_evaluator {
template <int K, class Functor, class... Evaluators>
auto make_map_evaluator_impl(const Functor& functor,
                             const Evaluators&... evaluators) {
  return MapEvaluator<Functor, Evaluators...>(functor, evaluators...);
}
}
}

template <int K, class Functor, class... Evaluators,
          CONCEPT_REQUIRES(
              const_algorithm::and_c<
                  execution_context::concept::k_compatible_evaluator<
                      K, Evaluators>()...>() &&
              echo::concept::callable<
                  Functor, type_traits::functor_return_type<Evaluators>...>() &&
              numeric_array::concept::compatible_functor_evaluators<
                  Functor, Evaluators...>())>
auto make_map_evaluator(const Functor& functor,
                        const Evaluators&... evaluators) {
  return detail::map_evaluator::make_map_evaluator_impl<K>(
      functor, fuse_evaluator<K, Evaluators...>(evaluators)...);
}
}
}

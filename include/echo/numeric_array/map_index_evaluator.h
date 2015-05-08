#pragma once

#include <echo/numeric_array/concept.h>

namespace echo {
namespace numeric_array {

/////////////////////////
// apply_odd_arguments //
/////////////////////////

namespace detail {
namespace map_index_evaluator {

template <class Functor, class ArgumentFirst, class ArgumentSecond>
decltype(auto) apply_odd_arguments(const Functor& functor,
                                   ArgumentFirst argument_first,
                                   ArgumentSecond argument_second) {
  return functor(argument_first);
}

template <class Functor, class ArgumentFirst, class ArgumentSecond,
          class... ArgumentsRest,
          CONCEPT_REQUIRES(sizeof...(ArgumentsRest) % 2 == 0)>
decltype(auto) apply_odd_arguments(const Functor& functor,
                                   ArgumentFirst argument_first,
                                   ArgumentSecond argument_second,
                                   ArgumentsRest... arguments_rest) {
  return apply_odd_arguments([&](auto... arguments) {
    return functor(argument_first, arguments...);
  }, arguments_rest...);
}
}
}

///////////////////////
// MapIndexEvaluator //
///////////////////////

namespace detail {
namespace map_index_evaluator {

template <class, class Derived>
struct MapIndexEvaluatorImpl {};

template <class Derived>
struct MapIndexEvaluatorImpl<std::index_sequence<0>, Derived> {
  decltype(auto) operator()(index_t i) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return derived.functor()(i);
  }
};

template <std::size_t... Indexes, class Derived>
struct MapIndexEvaluatorImpl<std::index_sequence<Indexes...>, Derived> {
  decltype(auto) operator()(
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return apply_odd_arguments(derived.functor(), indexes...);
  }
};
}
}

template <int K, class Functor>
class MapIndexEvaluator
    : public detail::map_index_evaluator::MapIndexEvaluatorImpl<
          std::make_index_sequence<(K == 1 ? 1 : 2 * K)>,
          MapIndexEvaluator<K, Functor>> {
 public:
  MapIndexEvaluator(const Functor& functor) : _functor(functor) {}
  const auto& functor() const { return _functor; }
 private:
  Functor _functor;
};

template<int K, class Functor, 
  CONCEPT_REQUIRES(concept::index_functor<K, Functor>())
>
auto make_map_index_evaluator(const Functor& functor) {
  return MapIndexEvaluator<K, Functor>(functor);
}

}
}

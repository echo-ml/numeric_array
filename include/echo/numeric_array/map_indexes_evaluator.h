#pragma once

#define DETAIL_NS detail_map_indexes_evaluator

#include <echo/numeric_array/concept.h>
#include <echo/repeat_type.h>

namespace echo {
namespace numeric_array {

/////////////////////////
// apply_odd_arguments //
/////////////////////////

namespace DETAIL_NS {
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

///////////////////////
// MapIndexEvaluator //
///////////////////////

namespace DETAIL_NS {
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
  decltype(auto) operator()(repeat_type_c<Indexes, index_t>... indexes) const {
    const auto& derived = static_cast<const Derived&>(*this);
    return apply_odd_arguments(derived.functor(), indexes...);
  }
};
}

template <int K, class Functor>
class MapIndexEvaluator : public DETAIL_NS::MapIndexEvaluatorImpl<
                              std::make_index_sequence<(K == 1 ? 1 : 2 * K)>,
                              MapIndexEvaluator<K, Functor>>,
                          htl::Pack<Functor> {
 public:
  MapIndexEvaluator(const Functor& functor) : htl::Pack<Functor>(functor) {}
  const auto& functor() const { return htl::unpack<Functor>(*this); }
};

template <int K, class Functor,
          CONCEPT_REQUIRES(concept::index_functor<K, Functor>())>
auto make_map_indexes_evaluator(const Functor& functor) {
  return MapIndexEvaluator<K, Functor>(functor);
}
}
}

#undef DETAIL_NS

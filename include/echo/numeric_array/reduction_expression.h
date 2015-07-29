#pragma once

#include <echo/numeric_array/expression.h>

namespace echo {
namespace numeric_array {
//------------------------------------------------------------------------------
// NumericArrayReductionExpression
//------------------------------------------------------------------------------
template <class Dimensionality, class Structure, class Identity, class Mapper,
          class Reducer>
class NumericArrayReductionExpression : htl::Pack<Dimensionality> {
 public:
  NumericArrayReductionExpression(const Dimensionality& dimensionality,
                                  const Identity& identity,
                                  const Mapper& mapper, const Reducer& reducer)
      : htl::Pack<Dimensionality>(dimensionality),
        _identity(identity),
        _mapper(mapper),
        _reducer(reducer) {}
  using structure = Structure;
  const Dimensionality& dimensionality() const {
    return htl::unpack<Dimensionality>(*this);
  }
  const Identity& identity() const { return _identity; }
  const Mapper& mapper() const { return _mapper; }
  const Reducer& reducer() const { return _reducer; }

 private:
  Identity _identity;
  Mapper _mapper;
  Reducer _reducer;
};

//------------------------------------------------------------------------------
// make_numeric_array_reduction_expression
//------------------------------------------------------------------------------
template <class Structure, class Dimensionality, class Identity, class Mapper,
          class Reducer>
auto make_numeric_array_reduction_expression(
    const Dimensionality& dimensionality, const Identity& identity,
    const Mapper& mapper, const Reducer& reducer) {
  return NumericArrayReductionExpression<Dimensionality, Structure, Identity,
                                         Mapper, Reducer>(
      dimensionality, identity, mapper, reducer);
}

//------------------------------------------------------------------------------
// make_reduction_expression
//------------------------------------------------------------------------------
template <class Identity, class Reducer, class Dimensionality, class Structure,
          class Evaluator,
          CONCEPT_REQUIRES(execution_context::concept::reduction_expression<
              NumericArrayReductionExpression<Dimensionality, Structure,
                                              Identity, Evaluator, Reducer>>())>
auto make_reduction_expression(
    numeric_array_expression_tag, const Identity& identity,
    const Reducer& reducer,
    const NumericArrayExpression<Dimensionality, Structure, Evaluator>&
        expression) {
  return make_numeric_array_reduction_expression<Structure>(
      expression.dimensionality(), identity, expression.evaluator(), reducer);
}
}
}

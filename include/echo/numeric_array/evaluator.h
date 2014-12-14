#pragma once

#include <tuple>
#include <echo/index.h>
#include <echo/k_array.h>

namespace echo { namespace numeric_array {

/////////////////////
// ScalarEvaluator //
/////////////////////

template<class Scalar>
class ScalarEvaluator {
 public:
  ScalarEvaluator(Scalar scalar) : _scalar(scalar) {}

  Scalar operator()(Index<1> index) const {
    return _scalar;
  }
 private:
  Scalar _scalar;
};

template<class Scalar>
auto make_scalar_evaluator(const Scalar& scalar) {
  return ScalarEvaluator<Scalar>(scalar);
}

/////////////////////////////
// BinaryFunctionEvaluator //
/////////////////////////////

template<class Functor, class LhsEvaluator, class RhsEvaluator>
class BinaryFunctionEvaluator 
  : Functor
{
 public:
  BinaryFunctionEvaluator(const LhsEvaluator& lhs_evaluator, const RhsEvaluator& rhs_evaluator)
    : _lhs_evaluator(lhs_evaluator)
    , _rhs_evaluator(rhs_evaluator)
  {}
  decltype(auto) operator()(Index<1> index) const {
    return Functor::operator()(_lhs_evaluator(index), _rhs_evaluator(index));
  }
 private:
  LhsEvaluator _lhs_evaluator;
  RhsEvaluator _rhs_evaluator;
};

template<class Functor, class LhsEvaluator, class RhsEvaluator>
auto make_binary_function_evaluator(const Functor& functor
                                  , const LhsEvaluator& lhs_evaluator
                                  , const RhsEvaluator& rhs_evaluator)
{
  return BinaryFunctionEvaluator<
      Functor
    , LhsEvaluator
    , RhsEvaluator
  >(
      lhs_evaluator
    , rhs_evaluator
  );
}

///////////////////////////
// NumericArrayEvaluator //
///////////////////////////

template<class Pointer>
class NumericArrayEvaluator {
 public:
  NumericArrayEvaluator(Pointer data) : _data(data) {}
  decltype(auto) operator()(Index<1> index) const {
    return *(_data + index);
  }
 private:
  Pointer _data;
};

template<class Pointer>
auto make_numeric_array_evaluator(Pointer data) {
  return NumericArrayEvaluator<Pointer>(data);
}

//////////////////////////////
// NumericArrayMapEvaluator //
//////////////////////////////

namespace detail {

template<class Function, int... Indexes, class... Evaluators>
decltype(auto) apply_map(fatal::constant_sequence<int, Indexes...>
                       , Index<1> index
                       , const Function& function
                       , const std::tuple<Evaluators...>& evaluators) 
{
  return function(std::get<Indexes>(evaluators)(index)...);
}

} //end namespace detail

template<class Function, class... Evaluators>
class NumericArrayMapEvaluator
  : Function
{
 public:
  NumericArrayMapEvaluator(const Function& function, const Evaluators&... evaluators)
    : Function(function)
    , _evaluators(evaluators...)
  {}
  decltype(auto) operator()(Index<1> index) const {
    return detail::apply_map(fatal::constant_range<int, 0, sizeof...(Evaluators)>()
                           , index
                           , static_cast<const Function&>(*this)
                           , _evaluators);
  }
 private:
  std::tuple<Evaluators...> _evaluators;  
};

template<class Function, class... Evaluators>
auto make_numeric_array_map_evaluator(const Function& function
                                    , const Evaluators&... evaluators)
{
  return NumericArrayMapEvaluator<Function, Evaluators...>(function, evaluators...);
}

}} //end namespace echo::numeric_array

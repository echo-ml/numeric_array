#pragma once

#include <echo/assert.h>
#include <echo/numeric_array/null_dimensionality.h>
#include <echo/numeric_array/evaluator.h>
#include <echo/numeric_array/scalar_evaluator.h>
#include <echo/numeric_array/numeric_array_evaluator.h>
#include <echo/numeric_array/conversion_evaluator.h>
#include <echo/numeric_array/expression_template_tag.h>
#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/structure_traits.h>
#include <echo/k_array.h>
#include <echo/execution_context.h>
#include <echo/expression_template.h>

namespace echo {
namespace numeric_array {

////////////////////////////
// NumericArrayExpression //
////////////////////////////

template <class Dimensionality, class Structure, class Evaluator>
class NumericArrayExpression
    : htl::Pack<Dimensionality>,
      public expression_template::ExpressionTemplateConstAssignment<
          NumericArrayExpression<Dimensionality, Structure, Evaluator>,
          numeric_array_expression_tag,
          evaluator_traits::value_type<Evaluator>> {
 public:
  NumericArrayExpression(const Dimensionality& dimensionality,
                         const Evaluator& evaluator)
      : htl::Pack<Dimensionality>(dimensionality), _evaluator(evaluator) {}
  using expression_template_tag = numeric_array_expression_tag;
  using structure = Structure;

  using expression_template::ExpressionTemplateConstAssignment<
      NumericArrayExpression, numeric_array_expression_tag,
      evaluator_traits::value_type<Evaluator>>::
  operator=;

  const Evaluator evaluator() const { return _evaluator; }
  const Dimensionality& dimensionality() const {
    return htl::unpack<Dimensionality>(*this);
  }

 private:
  Evaluator _evaluator;
};

template <class Structure, class Dimensionality, class Evaluator,
          CONCEPT_REQUIRES(
              execution_context::concept::structure<Structure>() &&
              k_array::concept::dimensionality<Dimensionality>() &&
              execution_context::concept::k_compatible_evaluator<
                  dimensionality_traits::num_dimensions<Dimensionality>(),
                  Evaluator>())>
auto make_numeric_array_expression(const Dimensionality& dimensionality,
                                   const Evaluator& evaluator) {
  return NumericArrayExpression<Dimensionality, Structure, Evaluator>(
      dimensionality, evaluator);
}

template <
    class Structure, class Dimensionality, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     std::is_same<Dimensionality, NullDimensionality>::value &&
                     execution_context::concept::flat_evaluator<Evaluator>())>
auto make_numeric_array_expression(const Dimensionality& dimensionality,
                                   const Evaluator& evaluator) {
  return NumericArrayExpression<Dimensionality, Structure, Evaluator>(
      dimensionality, evaluator);
}

/////////////////////
// make_expression //
/////////////////////

template <class NumericArray,
          CONCEPT_REQUIRES(concept::numeric_array<uncvref_t<NumericArray>>())>
auto make_expression(numeric_array_expression_tag,
                     NumericArray&& numeric_array) {
  return make_numeric_array_expression<
      expression_traits::structure<uncvref_t<NumericArray>>>(
      k_array::get_dimensionality(numeric_array),
      make_numeric_array_evaluator(numeric_array));
}

template <class Scalar,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>())>
auto make_expression(numeric_array_expression_tag, const Scalar& scalar) {
  return make_numeric_array_expression<structure::scalar>(
      NullDimensionality(), make_scalar_evaluator(scalar));
}

template <class Dimensionality, class Structure, class Evaluator>
auto make_expression(numeric_array_expression_tag,
                     const NumericArrayExpression<Dimensionality, Structure,
                                                  Evaluator>& expression) {
  return expression;
}
}
}  // end namespace echo::numeric_array

#pragma once

// #include <echo/numeric_array/expression.h>
// #include <echo/index.h>
// #include <tbb/tbb.h>
//
// namespace echo { namespace numeric_array {
//
// class NumericArrayExecuter {
//  public:
//   struct serial_mode {};
//   struct parallel_mode {};
//
//   template<class Shape, class Evaluator>
//   void operator()(serial_mode
//                 , const NumericArrayExpression<Shape, Evaluator>& expression) const 
//   {
//     execute_evaluation_range(expression.evaluator()
//                            , 0
//                            , get_num_elements(expression.shape()));
//   }
//   
//   template<class Shape, class Evaluator>
//   void operator()(parallel_mode
//                 , const NumericArrayExpression<Shape, Evaluator>& expression) const
//   {
//     tbb::parallel_for(tbb::blocked_range<int>(0, get_num_elements(expression.shape()))
//                     , [this, &expression](const tbb::blocked_range<int> range) {
//                         this->execute_evaluation_range(expression.evaluator()
//                                                      , range.begin()
//                                                      , range.end());
//                       });
//   }
//
//   template<class Shape, class Evaluator>
//   void operator()(const NumericArrayExpression<Shape, Evaluator>& expression) const {
//     this->operator()(parallel_mode(), expression);
//   }
//  private:
//   template<class Evaluator>
//   void execute_evaluation_range(const Evaluator& evaluator, int first, int last) const {
// #pragma simd
//     for(int i=first; i<last; ++i)
//       evaluator(Index<1>(i));
//   }
// };
//
// }} //end namespace echo::numeric_array

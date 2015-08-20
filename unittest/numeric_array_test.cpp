#include <echo/numeric_array/numeric_array.h>
#include <echo/numeric_array/numeric_array_view.h>
#include <echo/numeric_array/numeric_subarray.h>
#include <echo/numeric_array/iteration.h>
#include <echo/numeric_array/concept.h>
#include <echo/numeric_array/map_expression.h>
#include <echo/numeric_array/map_indexes_expression.h>
#include <echo/numeric_array/reduction_expression.h>
#include <echo/numeric_array/test.h>
#include <echo/numeric_array/copy.h>
#include <echo/numeric_array/print.h>
#include <echo/tbb_expression_executer.h>
#include <catch.hpp>
#include <numeric>

using namespace echo;
using namespace echo::numeric_array;

static const echo::execution_context::intel_tbb::ExpressionExecuter executer{};

TEST_CASE("numeric_array") {
  using Shape1 = ShapeC<3, 2>;
  using NumericArray1 = NumericArray<double, Shape1>;
  CHECK(numeric_array::concept::numeric_array<NumericArray1>());
  using Shape2 = ShapeC<3, 7>;
  using NumericArray2 = NumericArray<double, Shape2>;
  NumericArray1 array1{Shape1{}};
  NumericArray2 array2{Shape2{}};
  NumericArray1 array3{Shape1{}};
  std::iota(all_begin(array1), all_end(array1), 0);
  std::iota(all_begin(array3), all_end(array3), 1);

  auto array4 = make_numeric_array_view(
      array1.data(), k_array::make_subshape(make_dimensionality(2_index),
                                            make_strides(3_index)));

  SECTION("arithmetic") {
    auto expr1 = array1 + array1;
    // auto compilation_failure = array1 * array2; //static assertion failure
    auto expr2 = sqrt(array1);

    // auto expr3 = pow(array1, array1);

    CHECK(expr1.evaluator()(0) == 0);
    CHECK(expr1.evaluator()(1) == 2);

    CHECK(expr2.evaluator()(0) == 0);
    CHECK(expr2.evaluator()(1) == 1);
    CHECK(expr2.evaluator()(2) == std::sqrt(2.0));
  }

  SECTION("assign/move") {
    array3 = array1;
    array3 = std::move(array1);
  }

  SECTION("expression assignment") {
    auto assign1 = (array3 = sqrt(array1));
    auto assign2 = (array3 += array1);

    CHECK(assign1.evaluator()(2) == std::sqrt(2));
    CHECK(assign2.evaluator()(1) == 3);
  }

  SECTION("reduction expression") {
    auto expr = sum_elements(array1);
    CHECK(executer(expr) == 5 * 6 / 2);
  }

  SECTION("scalar assignment") {
    auto assign1 = array3 = 7;
    executer(assign1);
    CHECK(array3(0, 0) == 7);
  }

  SECTION("serial evaluator") {
    auto assign1 = (array3 = sqrt(array1));
    executer(assign1);
    REQUIRE(assign1.evaluator()(0) == std::sqrt(0));
    REQUIRE(assign1.evaluator()(5) == std::sqrt(5));
  }

  SECTION("subarray") {
    std::fill_n(array1.data(), int(get_num_elements(array1)), 0);
    auto expr = array4 =
        map_indexes([](index_t i) { return (i + 1) * (i + 1); }, 2_index);
    executer(expr);

    CHECK(array1(0, 0) == 1);
    CHECK(array1(0, 1) == 4);
  }
}

TEST_CASE("make_numeric_array") {
  auto n1 = make_numeric_array<double>(make_dimensionality(3_index, 4_index));
  auto n2 =
      make_static_numeric_array<double>(make_dimensionality(3_index, 4_index));
  auto n3 = make_static_numeric_array(executer, n1);
}

TEST_CASE("construction") {
  NumericArray<double, Shape<StaticIndex<1>, index_t>> n1(
      make_shape(1_index, 10));
  CHECK(get_num_elements(n1) == 10);

  n1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  CHECK(n1(0) == 1);
  CHECK(n1(9) == 10);
}

TEST_CASE("accessor") {
  NumericArray<double, ShapeC<2, 3>> n1;
  const auto& n1_cref = n1;
  n1(0, 1) = 7;
  CHECK(n1_cref(0, 1) == 7);
}

TEST_CASE("1-d accessor") {
  NumericArray<double, ShapeC<3>> n1;
  n1(0) = 1;
  n1(1) = 2;
  n1(2) = 3;
  auto values123 = {1, 2, 3};
  CHECK(
      std::equal(std::begin(values123), std::end(values123), n1.const_data()));

  NumericArray<double, ShapeC<1, 3>> n2;
  n2(0) = 1;
  n2(1) = 2;
  n2(2) = 3;
  CHECK(
      std::equal(std::begin(values123), std::end(values123), n2.const_data()));
  CHECK(n2(0, 0) == 1);

  double data[] = {1, 2, 3, 4, 5, 6};
  auto v1 = make_numeric_array_view(
      data, make_subshape(make_dimensionality(1_index, 2_index),
                          make_strides(1_index, 3_index)));
  CHECK(v1(0) == 1);
  CHECK(v1(1) == 4);
  CHECK(v1(0, 1) == 4);
}

TEST_CASE("initialization") {
  NumericArray<double, ShapeC<2, 3>> n1;
  n1 = {{1, 2, 3}, {4, 5, 6}};
  CHECK(n1(0, 0) == 1);
  CHECK(n1(1, 2) == 6);

  auto v1 = make_view(n1);
  v1 = {{2, 3, 4}, {5, 6, 7}};
  CHECK(v1(0, 0) == 2);
  CHECK(v1(1, 2) == 7);
}

TEST_CASE("array_equal") {
  NumericArray<double, ShapeC<2, 3>> n1;
  n1 = {{1, 2, 3}, {4, 5, 6}};
  ARRAY_EQUAL(n1, {{1, 2, 3}, {4, 5, 6}});

  NumericArray<double, ShapeC<3, 1>> n2;
  n2 = {4, 5, 6};
  ARRAY_EQUAL(n2, {{4}, {5}, {6}});
}

TEST_CASE("copyable") {
  NumericArray<double, ShapeC<3, 2>> n1, n2;
  n1 = {{1, 2}, {3, 4}, {5, 6}};
  auto v1 = make_numeric_array_view(
      n1.data(), make_subshape(n1.shape(), slice::counted_range(0, 2_index),
                               slice::counted_range(0, 2_index)));
  auto v2 = make_numeric_array_view(
      n2.data(), make_subshape(n2.shape(), slice::counted_range(0, 2_index),
                               slice::counted_range(0, 2_index)));

  NumericArray<double, ShapeC<3, 2>> n3;
  NumericArray<float, ShapeC<3, 2>> n4;
  n3 = {{1, 2}, {3, 4}, {5, 6}};
  n4 = {{2, 3}, {4, 5}, {6, 7}};
  SECTION("contiguous_copy") {
    copy(executer, n1, n2);
    ARRAY_EQUAL(n2, {{1, 2}, {3, 4}, {5, 6}});
  }

  SECTION("noncontiguous_copy") {
    copy(executer, v1, v2);
    ARRAY_EQUAL(v2, {{1, 2}, {3, 4}});
  }

  SECTION("convertible_copy1") {
    copy(executer, n3, n4);
    ARRAY_EQUAL(n4, {{1, 2}, {3, 4}, {5, 6}});
  }
  SECTION("convertible_copy1") {
    copy(executer, n4, n3);
    ARRAY_EQUAL(n3, {{2, 3}, {4, 5}, {6, 7}});
  }
}

TEST_CASE("numeric_subarray") {
  NumericArray<double, ShapeC<3, 4>> n1;
  n1 = {{1, 2, 3, 4}, {4, 5, 6, 7}, {8, 9, 10, 11}};

  auto s1 = make_numeric_subarray(n1, slice::all, slice::all);
  ARRAY_EQUAL(s1, {{1, 2, 3, 4}, {4, 5, 6, 7}, {8, 9, 10, 11}});

  auto s2 = make_numeric_subarray(n1, slice::all, slice::counted_range(1, 3));
  ARRAY_EQUAL(s2, {{2, 3, 4}, {5, 6, 7}, {9, 10, 11}});

  auto s3 = make_numeric_subarray(n1, slice::range(1, 3), slice::all);
  ARRAY_EQUAL(s3, {{4, 5, 6, 7}, {8, 9, 10, 11}});

  auto s4 = make_numeric_subarray(n1, 1, slice::all);
  ARRAY_EQUAL(s4, {4, 5, 6, 7});
}

TEST_CASE("numeric_array_equal") {
  NumericArray<double, ShapeC<2, 3>> n1;
  n1 = {{1, 2, 3}, {4, 5, 6}};
  ARRAY_EQUAL(n1, n1);
  ARRAY_EQUAL(n1, make_cview(n1));
}

TEST_CASE("print_numeric_array") {
  NumericArray<int, ShapeC<2>> n1;
  n1 = {1, 2};
  CHECK(to_string(n1) == "{1, 2}");

  NumericArray<int, ShapeC<3, 2>> n2;
  n2 = {{1, 2}, {3, 4}, {5, 6}};
  CHECK(to_string(n2) == "{{1, 2}, {3, 4}, {5, 6}}");
}

TEST_CASE("1-element numeric array") {
  NumericArray<double, ShapeC<1, 1>> n1;
  n1 = {3};
  CHECK(n1(0) == 3);
}

TEST_CASE("iteration") {
  NumericArray<int, ShapeC<6, 1>> n1;
  n1 = {1, 2, 3, 4, 5, 6};
  auto first1 = begin(n1);
  auto last1 = end(n1);
  CHECK(*first1 == 1);
  CHECK(std::distance(first1, last1) == 6);

  auto n2 = make_numeric_array_view(
      n1.data(), make_subshape(make_dimensionality(2_index, 1_index),
                               make_strides(3_index, 3_index)));
  auto first2 = cbegin(n2);
  auto last2 = cend(n2);
  CHECK(*first2 == 1);
  CHECK(*std::next(first2) == 4);
  CHECK(std::distance(first2, last2) == 2);

  NumericArray<int, ShapeC<1, 1>> n3;
  n3 = {22};
  auto first3 = begin(n3);
  auto last3 = end(n3);
  CHECK(*first3 == 22);
  CHECK(std::next(first3) == last3);
}

TEST_CASE("expression_iterator") {
  NumericArray<double, ShapeC<3>> v1, v2;
  v1 = {3, 4, 5};
  v2 = {4, 5, 6};

  auto e1 = v1*v2;
  auto i = begin(e1);
  auto last = end(e1);
  CHECK(*i++ == 12);
  CHECK(*i++ == 20);
  CHECK(*i++ == 30);
  CHECK(i == last);
}

#pragma once

namespace echo {
namespace numeric_array {

namespace access_mode {
struct readonly_t {};
struct readwrite_t {};
struct raw_t {};
constexpr readonly_t readonly{};
constexpr readwrite_t readwrite{};
constexpr raw_t raw{};
}

///////////////////////////////
// NumericArrayConstAccessor //
///////////////////////////////

namespace detail {
namespace numeric_array_accessor {

template<class Derived, class Shape,
   bool HasSingleFreeDimension>
struct NumericArrayConstSingleIndexAccessorImpl {
  template<bool X = true, std::enable_if_t<X && false, int> = 0>
  void operator()() const {}
};

template<class Derived, class Shape>
struct NumericArrayConstSingleIndexAccessorImpl<
  Derived, Shape, true>
{
  decltype(auto) operator()(access_mode::readonly_t, index_t index) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return *(derived.const_data() +
             index *
                 get_stride<shape_traits::free_dimension<Shape>()>(derived));
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t index) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return *(derived.data() +
             index *
                 get_stride<shape_traits::free_dimension<Shape>()>(derived));
  }
  decltype(auto) operator()(access_mode::raw_t, index_t index) const {
    return this->operator()(access_mode::readwrite, index);
  }
  decltype(auto) operator()(index_t index) const {
    return this->operator()(access_mode::readwrite, index);
  }

};

template <class Indexes, class Derived, class BaseArray, class Shape,
          class Structure>
struct NumericArrayConstAccessorImpl {};

template <std::size_t... Indexes, class Derived, class BaseArray, class Shape,
          class Structure>
struct NumericArrayConstAccessorImpl<std::index_sequence<Indexes...>, Derived,
                                     BaseArray, Shape, Structure> 
  : NumericArrayConstSingleIndexAccessorImpl<
      Derived, Shape,
       shape_traits::num_free_dimensions<Shape>() == 1 &&
       shape_traits::num_dimensions<Shape>() != 1
    >
{
  using NumericArrayConstSingleIndexAccessorImpl<
    Derived, Shape,
      shape_traits::num_free_dimensions<Shape>() == 1 &&
      shape_traits::num_dimensions<Shape>() != 1>::operator();
  decltype(auto) operator()(
      access_mode::readonly_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.BaseArray::operator()(indexes...);
  }
  decltype(auto) operator()(
      access_mode::readwrite_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.BaseArray::operator()(indexes...);
  }
  decltype(auto) operator()(
      access_mode::raw_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.BaseArray::operator()(indexes...);
  }
  decltype(auto) operator()(
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    return this->operator()(access_mode::readwrite, indexes...);
  }

  // intel c++ compiler has a bug with member-template functions like
  // this, so use inheritance versions instead
  // CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
  //                         shape_traits::num_dimensions<Shape>() != 1)
  // decltype(auto) operator()(access_mode::readonly_t, index_t index) const {
  //   const Derived& derived = static_cast<const Derived&>(*this);
  //   return *(derived.const_data() +
  //            index *
  //                get_stride<shape_traits::free_dimension<Shape>()>(derived));
  // }
  // CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
  //                         shape_traits::num_dimensions<Shape>() != 1)
  // decltype(auto) operator()(access_mode::readwrite_t, index_t index) const {
  //   const Derived& derived = static_cast<const Derived&>(*this);
  //   return *(derived.data() +
  //            index *
  //                get_stride<shape_traits::free_dimension<Shape>()>(derived));
  // }
  // CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
  //                         shape_traits::num_dimensions<Shape>() != 1)
  // decltype(auto) operator()(access_mode::raw_t, index_t index) const {
  //   return this->operator()(access_mode::readwrite, index);
  // }
  // CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
  //                         shape_traits::num_dimensions<Shape>() != 1)
  // decltype(auto) operator()(index_t index) const {
  //   return this->operator()(access_mode::readwrite, index);
  // }
};
}
}

template <class Derived, class BaseArray, class Shape, class Structure>
struct NumericArrayConstAccessor
    : detail::numeric_array_accessor::NumericArrayConstAccessorImpl<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Derived, BaseArray, Shape, Structure> {};

//////////////////////////
// NumericArrayAccessor //
//////////////////////////

namespace detail {
namespace numeric_array_accessor {

template <class Reference, class ConstReturn>
struct return_type_impl {};

template <class T>
struct return_type_impl<T, T> {
  using type = T;
};

template <class T>
struct return_type_impl<T&, const T&> {
  using type = T&;
};

template <class T>
struct return_type_impl<T&, T> {
  using type = T;
};

template <class Reference, class ConstReturn>
using return_type = typename return_type_impl<Reference, ConstReturn>::type;

template <class Indexes, class Derived, class BaseArray, class Shape,
          class Structure>
struct NumericArrayAccessorImpl {};

template <std::size_t... Indexes, class Derived, class BaseArray, class Shape,
          class Structure>
struct NumericArrayAccessorImpl<std::index_sequence<Indexes...>, Derived,
                                BaseArray, Shape, Structure>
    : NumericArrayConstAccessor<Derived, BaseArray, Shape, Structure> {
  using NumericArrayConstAccessor<Derived, BaseArray, Shape, Structure>::
  operator();

  decltype(auto) operator()(
      access_mode::readonly_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::readonly, indexes...));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::readonly, indexes...));
  }
  decltype(auto) operator()(
      access_mode::readwrite_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::readwrite, indexes...));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::readwrite, indexes...));
  }
  decltype(auto) operator()(
      access_mode::raw_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::raw, indexes...));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::raw, indexes...));
  }
  decltype(auto) operator()(
      std::enable_if_t<Indexes || true, index_t>... indexes) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn = decltype(const_derived.operator()(indexes...));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(indexes...));
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  decltype(auto) operator()(access_mode::readonly_t, index_t index) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::readonly, index));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::readonly, index));
  }
  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  decltype(auto) operator()(access_mode::readwrite_t, index_t index) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::readwrite, index));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::readwrite, index));
  }
  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  decltype(auto) operator()(access_mode::raw_t, index_t index) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::raw, index));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::raw, index));
  }
  CONCEPT_MEMBER_REQUIRES(shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1)
  decltype(auto) operator()(index_t index) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn = decltype(const_derived.operator()(index));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(index));
  }
};
}
}

template <class Derived, class BaseArray, class Shape, class Structure>
struct NumericArrayAccessor
    : detail::numeric_array_accessor::NumericArrayAccessorImpl<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Derived, BaseArray, Shape, Structure> {};
}
}

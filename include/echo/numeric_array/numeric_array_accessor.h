#pragma once

#define DETAIL_NS detail_numeric_array_accessor

#include <echo/access_mode.h>
#include <type_traits>
#include <utility>

namespace echo {
namespace numeric_array {

/////////////////
// return_type //
/////////////////

namespace DETAIL_NS {
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
}

//////////////////////////////////////////
// NumericArraySingleIndexConstAccessor //
//////////////////////////////////////////

namespace DETAIL_NS {
template <class Derived, class Shape, bool HasSingleFreeDimension>
struct NumericArraySingleIndexConstAccessor {
  template <bool X = true, std::enable_if_t<X && false, int> = 0>
  void operator()() const {}
};

template <class Derived, class Shape>
struct NumericArraySingleIndexConstAccessor<Derived, Shape, true> {
  decltype(auto) operator()(access_mode::readonly_t, index_t index) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return *(derived.const_data() +
             index *
                 get_stride<shape_traits::free_dimension<Shape>()>(derived));
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t index) const {
    return this->operator()(access_mode::raw, index);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t index) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return *(derived.data() +
             index *
                 get_stride<shape_traits::free_dimension<Shape>()>(derived));
  }
  decltype(auto) operator()(index_t index) const {
    return this->operator()(access_mode::readwrite, index);
  }
};
}

/////////////////////////////////////
// NumericArraySingelIndexAccessor //
/////////////////////////////////////

namespace DETAIL_NS {
template <class Derived, class Shape, bool HasSingleFreeDimension>
struct NumericArraySingleIndexAccessor {
  template <bool X = true, std::enable_if_t<X && false, int> = 0>
  void operator()() const {}
};

template <class Derived, class Shape>
struct NumericArraySingleIndexAccessor<Derived, Shape, true>
    : NumericArraySingleIndexConstAccessor<Derived, Shape, true> {
  using NumericArraySingleIndexConstAccessor<Derived, Shape, true>::operator();
  decltype(auto) operator()(access_mode::readwrite_t, index_t index) {
    return this->operator()(access_mode::raw, index);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t index) {
    const Derived& const_derived = static_cast<const Derived&>(*this);
    Derived& derived = static_cast<Derived&>(*this);
    using Reference = decltype(*derived.data());
    using ConstReturn =
        decltype(const_derived.operator()(access_mode::readwrite, index));
    return const_cast<return_type<Reference, ConstReturn>>(
        const_derived.operator()(access_mode::readwrite, index));
  }
  decltype(auto) operator()(index_t index) {
    return this->operator()(access_mode::readwrite, index);
  }
};
}

///////////////////////////////
// NumericArrayConstAccessor //
///////////////////////////////

namespace DETAIL_NS {
template <class Indexes, class Derived, class Shape, class Structure>
struct NumericArrayConstAccessorImpl {};

template <std::size_t... Indexes, class Derived, class Shape, class Structure>
struct NumericArrayConstAccessorImpl<std::index_sequence<Indexes...>, Derived,
                                     Shape, Structure> {
  decltype(auto) operator()(
      access_mode::readonly_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.k_array()(access_mode::readonly, indexes...);
  }
  decltype(auto) operator()(
      access_mode::readwrite_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    return this->operator()(access_mode::raw, indexes...);
  }
  decltype(auto) operator()(
      access_mode::raw_t,
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.k_array()(indexes...);
  }
  decltype(auto) operator()(
      std::enable_if_t<Indexes || true, index_t>... indexes) const {
    return this->operator()(access_mode::readwrite, indexes...);
  }
};
}

template <class Derived, class Shape, class Structure>
struct NumericArrayConstAccessor
    : DETAIL_NS::NumericArrayConstAccessorImpl<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Derived, Shape, Structure>,
      DETAIL_NS::NumericArraySingleIndexConstAccessor<
          Derived, Shape, shape_traits::num_free_dimensions<Shape>() == 1 &&
                              shape_traits::num_dimensions<Shape>() != 1> 
{
  using DETAIL_NS::NumericArrayConstAccessorImpl<
      std::make_index_sequence<shape_traits::num_dimensions<Shape>()>, Derived,
      Shape, Structure>::
  operator();
  using DETAIL_NS::NumericArraySingleIndexConstAccessor<
      Derived, Shape, shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1>::
  operator();
};

//////////////////////////
// NumericArrayAccessor //
//////////////////////////

namespace DETAIL_NS {
template <class Indexes, class Derived, class Shape, class Structure>
struct NumericArrayAccessorImpl {};

template <std::size_t... Indexes, class Derived, class Shape, class Structure>
struct NumericArrayAccessorImpl<std::index_sequence<Indexes...>, Derived, Shape,
                                Structure>
    : NumericArrayConstAccessorImpl<std::index_sequence<Indexes...>, Derived,
                                    Shape, Structure> {
  using NumericArrayConstAccessorImpl<std::index_sequence<Indexes...>, Derived,
                                      Shape, Structure>::
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
};
}

template <class Derived, class Shape, class Structure>
struct NumericArrayAccessor
    : DETAIL_NS::NumericArrayAccessorImpl<
          std::make_index_sequence<shape_traits::num_dimensions<Shape>()>,
          Derived, Shape, Structure>,
      DETAIL_NS::NumericArraySingleIndexAccessor<
          Derived, Shape, shape_traits::num_free_dimensions<Shape>() == 1 &&
                              shape_traits::num_dimensions<Shape>() != 1> {
  using DETAIL_NS::NumericArrayAccessorImpl<
      std::make_index_sequence<shape_traits::num_dimensions<Shape>()>, Derived,
      Shape, Structure>::
  operator();
  using DETAIL_NS::NumericArraySingleIndexAccessor<
      Derived, Shape, shape_traits::num_free_dimensions<Shape>() == 1 &&
                          shape_traits::num_dimensions<Shape>() != 1>::
  operator();
};
}
}

#undef DETAIL_NS
